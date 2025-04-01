#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
        if (argc != 2) {
                fprintf(stderr, "Usage: ./client.c <client_name>\n");
                return EXIT_FAILURE;
        }

        char fifo_path[1000];
        snprintf(fifo_path, sizeof(fifo_path), "/tmp/%s_csbb8841_fifo", argv[1]);

        int fifo_file_descriptor = open(fifo_path, O_WRONLY);
        if (fifo_file_descriptor == -1) {
                fprintf(stderr, "error opening fifo\n");
                return EXIT_FAILURE;
        }

        char message[1000];
        while(1) {
                printf("Message: ");
                if (fgets(message, sizeof(message), stdin) == NULL) {
                        fprintf(stderr, "error reading message from stdin\n");
                        return EXIT_FAILURE;
                }

                if (write(fifo_file_descriptor, message, strlen(message)) != (ssize_t)strlen(message)) {
                        fprintf(stderr, "error sending message");
                        return EXIT_FAILURE;
                }

                if (strlen(message) == 1) {
                        printf("disconnect\n");
                        break;
                }
        }

        close(fifo_file_descriptor);

        return EXIT_SUCCESS;
}
