#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "server.h"
#include "worker.h"

int input_pipe[2];
int output_pipe[2];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void run_server() {
	int server_fd, client_fd;
	struct sockaddr_in addr;
	char buffer[100];

	pipe(input_pipe);
	pipe(output_pipe);

	pthread_t t1, t2;
	pthread_create(&t1, NULL, worker_thread, NULL);
	pthread_create(&t2, NULL, worker_thread, NULL);

	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family= AF_INET;
	addr.sin_port = htons(5050);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
	listen(server_fd, 1);

	printf("Server running on port 5050\n");

	client_fd = accept(server_fd, NULL, NULL);

	while(1) {
		read(client_fd, buffer, sizeof(buffer));
		write(input_pipe[1], buffer, strlen(buffer)+1);

		read(output_pipe[0], buffer, sizeof(buffer));
		write(client_fd, buffer, strlen(buffer)+1);

		if (strcmp(buffer, "exit") == 0)
			break;
	}

	close(client_fd);
	close(server_fd);
}



