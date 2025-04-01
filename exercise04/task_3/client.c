#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LEN 1024
#define MAX_MSG_LEN 1000

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <client_name>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char fifo_path[MAX_PATH_LEN];
	snprintf(fifo_path, sizeof(fifo_path), "/tmp/%s_csbb8841_fifo", argv[1]);

	int fifo_fd = open(fifo_path, O_WRONLY);
	if (fifo_fd == -1) {
		perror("Error opening FIFO");
		return EXIT_FAILURE;
	}

	char message[MAX_MSG_LEN];
	while (1) {
		printf("Message: ");
		if (!fgets(message, sizeof(message), stdin)) {
			fprintf(stderr, "Error reading message from stdin\n");
			return EXIT_FAILURE;
		}

		size_t msg_len = strlen(message);
		if (msg_len > 0 && message[msg_len - 1] == '\n') {
			message[msg_len - 1] = '\n';
		}

		if (write(fifo_fd, message, msg_len) != (ssize_t)msg_len) {
			perror("Error sending message");
			return EXIT_FAILURE;
		}

		if (msg_len == 1) {
			printf("Disconnecting...\n");
			break;
		}
	}

	close(fifo_fd);
	return EXIT_SUCCESS;
}
