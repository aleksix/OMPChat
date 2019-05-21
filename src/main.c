#include <stdio.h>
#include <getopt.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "client.h"

int g_close = 0;

#define DEFAULT_PORT htons(5000)
#define DEFAULT_IP htonl(INADDR_LOOPBACK)

int display_usage(const char *error)
{
	printf("USAGE: ompchat [-h] [-s] [-i] ip [-p] port\n");
	printf("Simple server-client chat written as an exercise.\n");
	printf("Arguments:\n");
	printf("-h, --help\t show this message\n");
	printf("-s, --server\t start as a server\n");
	printf("-i, --ip\t (as a client) set the ip of the server\n");
	printf("-p, --port\t (as a server) Choose the server port. (as a client) Specify the port on which to connect\n");

	if (error != NULL)
		printf("ERROR: %s\n\n", error);

	// Most often we'll want to return after showing the usage
	return 1;
}

int main(int argc, char *argv[])
{
	// Command-line argument parsing
	const struct option options[] = {
			{"help",   no_argument,       NULL, 'h'},
			{"server", no_argument,       NULL, 's'},
			{"ip",     required_argument, NULL, 'i'},
			{"port",   required_argument, NULL, 'p'},
	};

	int long_opt_index = 0;
	int opt = getopt_long(argc, argv, "hsi:p:", options, &long_opt_index);
	unsigned char is_server = 0;
	unsigned long ip = DEFAULT_IP;
	unsigned short port = DEFAULT_PORT;

	while (opt != -1)
	{
		switch (opt)
		{
			case 'h':
				return display_usage(NULL);
			case 's':
				is_server = 1;
				break;
			case 'i':
				inet_pton(AF_INET, optarg, &ip);
				break;
			case 'p':
				sscanf(optarg, "%hu", &port);
				port = htons(port);
				break;
			default:
				return display_usage("Unknown error during argument passing");
		}
		opt = getopt_long(argc, argv, "hsi:p:", options, &long_opt_index);
	}

	// Primary code
	pthread_t server_listen_thread;
	if (is_server == 1)
		pthread_create(&server_listen_thread, NULL, handle_server, port);

	handle_client(ip, port);

	if (is_server == 1)
		pthread_join(server_listen_thread, NULL);

	return 0;
}