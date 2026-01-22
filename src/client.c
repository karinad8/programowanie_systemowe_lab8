#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
	int sockfd;
	struct sockaddr_in addr;
	char buffer[100];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(5050);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

	while(1) {
		printf("Client input: ");
		scanf("%s", buffer);

		write(sockfd, buffer, strlen(buffer)+1);
		read(sockfd, buffer, sizeof(buffer));

		printf("Server's response: %s\n", buffer);

		if (strcmp(buffer, "exit") == 0)
			break;
	}
	close(sockfd);
	return 0;
}
