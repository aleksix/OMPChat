#include "server.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common.h"

extern int g_close;

void *handle_server(void *port)
{
	// fd_set to store client sockets
	fd_set clients;
	// fd_set for information from select()
	fd_set read_mask;
	// Maximum socket for select()
	int maxSocket;

	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = (int) port;
	addr.sin_family = AF_INET;
	// Constant for setting socket options
	int yes = 1;
	//Variable for storing various results for error-checking
	int res = 0;

	// Enable port and address reuse
	CHECK_ERROR(res, setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)), "Server SO_REUSEADDR error")
	CHECK_ERROR(res, setsockopt(server_sock, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes)), "Server SO_REUSEPORT error")

	// Bind the server socket and start listening
	CHECK_ERROR(res, bind(server_sock, (struct sockaddr *) &addr, sizeof(addr)), "Server socket binding error")
	CHECK_ERROR(res, listen(server_sock, 1024), "Server socket listening error")

	// Add the server socket to the listening set
	FD_SET(server_sock, &clients);
	maxSocket = server_sock;
	struct timeval timeout;
	timeout.tv_sec = 5;

	printf("Server initialized");

	// Main server loop
	while (g_close == 0)
	{
		int connection_closed = 0;

		// The read_mask needs to be reset properly every time
		memcpy(&read_mask, &clients, sizeof(clients));

		// TODO: Pollfd might be potentially more efficient, but it requires implementing a vector for the clients
		// Should be good enough for now.
		int ready;

		CHECK_ERROR(ready, select(maxSocket + 1, &read_mask, NULL, NULL, &timeout), "Server select() error")

		for (int c = 0; c <= maxSocket && ready > 0; ++c)
		{
			if (FD_ISSET(c, &read_mask))
			{
				if (c == server_sock)
				{
					// New connections inbound, accept them
					// TODO: Small race condition here - client can drop connection between select() and accept()
					//  meaning that the accept() will block.
					int client_sock;
					CHECK_ERROR(client_sock, accept(server_sock, NULL, NULL), "Server client connection error")
					FD_SET(client_sock, &clients);
					if (client_sock > maxSocket)
						maxSocket = client_sock;
					printf("Server: New connection on socket: %d\n", client_sock);
				}
				else
				{
					// Data on the client side
					char buf[256];
					connection_closed = 0;
					int bytes = read(c, buf, sizeof(buf));
					if (bytes <= 0)
					{
						// Connection closed
						if (bytes == -1)
							error("Socket read error");
						else
							printf("Server: Socket %d disconnected\n", c);
						FD_CLR(c, &clients);
						connection_closed = 1;
					}
					else
					{
						for (int c2 = 0; c2 <= maxSocket; ++c2)
						{
							if (FD_ISSET(c2, &clients) == 1 && c2 != server_sock)
							{
								send(c2, buf, sizeof(buf), 0);
							}
						}
					}

					if (connection_closed == 1)
					{
						if (c == maxSocket)
							while (FD_ISSET(maxSocket, &clients) == 0)
								--maxSocket;
					}
				}
				--ready;
			}
		}
	}

	close(server_sock);
	return NULL;
}