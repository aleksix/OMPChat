#include "client.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

extern int g_close;

int error(char *text)
{
	printf("%s %d: %s", text, errno, strerror(errno));
	return 1;
}

int handle_client(int ip, int port)
{
	// Buffers
	// Buffer for reading from the keyboard
	char write_buf[256];
	// Buffer for writing to the socket
	char read_buf[256];
	// Name buffer
	char name[128];
	int name_len;
	// Get and prepare a name for showing
	printf("Enter your nickname: ");
	scanf("%s", name);
	name[strlen(name) + 1] = 0;
	name[strlen(name)] = ':';

	name_len = strlen(name);

	// Variable for storing different results for error-checking
	int res = 0;

	// Create a socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_addr.s_addr = ip;
	addr.sin_family = AF_INET;
	addr.sin_port = port;

	// Try to connect to the server
	res = connect(sock, (struct sockaddr *) &addr, sizeof(addr));

	if (res < 0)
	{
		error("Connection error");
		return 1;
	}

	printf("Successfully connected to the server\n");

	// fd_set for select(). Contains the socket and the stdin
	fd_set read_info;
	int fd_stdin = STDIN_FILENO;

	int max_fd = sock;
	if (fd_stdin > max_fd)
		max_fd = fd_stdin;

	FD_ZERO(&read_info);

	while (g_close == 0)
	{
		FD_SET(sock, &read_info);
		FD_SET(fd_stdin, &read_info);
		res = select(max_fd + 1, &read_info, NULL, NULL, NULL);

		if (res < 0)
		{
			error("Input error");
			return 1;
		}

		if (FD_ISSET(sock, &read_info) == 1)
		{
			// We have info to read from the socket
			int buf_read = read(sock, read_buf, sizeof(read_buf));
			if (buf_read == 0)
			{
				g_close = 1;
				printf("Connection closed.\n");
				return 1;
			}
			else if (buf_read == -1)
			{
				g_close = 1;
				error("Socket reading error");
				return 1;
			}
			else
				printf("%s\n", read_buf);
		}
		if (FD_ISSET(fd_stdin, &read_info) == 1)
		{
			// We have info to send on the socket
			int buf_len = read(fd_stdin, write_buf + name_len, sizeof(write_buf) - name_len);

			if (buf_len < 0)
			{
				error("Keyboard error");
				return 1;
			}

			memcpy(write_buf, name, name_len);
			buf_len += name_len;
			printf("\n");

			// Remove newlines
			if (write_buf[buf_len - 1] == '\n')
			{
				write_buf[buf_len - 1] = 0;
				--buf_len;
			}

			if (strcmp(write_buf, "exit") != 0)
			{
				write(sock, write_buf, buf_len + 1);
			}
			else
				g_close = 1;

		}
	}

	close(sock);

	g_close = 1;
	return 0;
}