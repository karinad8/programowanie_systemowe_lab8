#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "worker.h"

extern int input_pipe[2];
extern int output_pipe[2];
extern pthread_mutex_t mutex;

void* worker_thread(void* arg) {
	char buffer[100];

	while(1) {
		read(input_pipe[0], buffer, sizeof(buffer));

		pthread_mutex_lock(&mutex);

		if (strcmp(buffer, "exit") == 0) {
			write(output_pipe[1], buffer, strlen(buffer)+1);
			pthread_mutex_unlock(&mutex);
			break;
		}

		buffer[0] = 'X';

		write(output_pipe[1], buffer, strlen(buffer)+1);

		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
