#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/poll.h>

#define MAX_PATH_LEN 1024
#define MAX_NAME_LEN 128

void clean_resources(int count, int *fds, char paths[][MAX_PATH_LEN]) {
    for (int i = 0; i < count; i++) {
        if (fds[i] != -1) {
            close(fds[i]);
        }
        unlink(paths[i]);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <client1> ... <clientN>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int client_count = argc - 1;
    int active_clients = 0;
    char fifo_paths[client_count][MAX_PATH_LEN];
    char client_names[client_count][MAX_NAME_LEN];
    int fifo_fds[client_count];
    struct pollfd poll_fds[client_count];
    int message_counter = 0;

    for (int i = 0; i < client_count; i++) {
        snprintf(client_names[i], MAX_NAME_LEN, "%s", argv[i + 1]);
        snprintf(fifo_paths[i], MAX_PATH_LEN, "/tmp/%s_csbb8841_fifo", client_names[i]);

        if (access(fifo_paths[i], F_OK) == 0) {
                unlink(fifo_paths[i]);
        }

        if (mkfifo(fifo_paths[i], 0666) == -1) {
            perror("Error creating FIFO");
            clean_resources(i, fifo_fds, fifo_paths);
            return EXIT_FAILURE;
        }

        fifo_fds[i] = open(fifo_paths[i], O_RDONLY);
        if (fifo_fds[i] == -1) {
            perror("Error opening FIFO");
            clean_resources(i, fifo_fds, fifo_paths);
            return EXIT_FAILURE;
        }

        poll_fds[i].fd = fifo_fds[i];
        poll_fds[i].events = POLLIN;
        printf("%s connected.\n", client_names[i]);
        active_clients++;
    }

    while (active_clients > 0) {
        if (poll(poll_fds, client_count, -1) == -1) {
            perror("Error with poll");
            clean_resources(client_count, fifo_fds, fifo_paths);
            return EXIT_FAILURE;
        }

        for (int i = 0; i < client_count; i++) {
            if (poll_fds[i].revents & POLLIN) {
                char buffer[MAX_PATH_LEN];
                ssize_t bytes_read = read(fifo_fds[i], buffer, sizeof(buffer) - 1);

                if (bytes_read <= 1) {
                    printf("%s disconnected.\n", client_names[i]);
                    close(fifo_fds[i]);
                    unlink(fifo_paths[i]);
                    poll_fds[i].fd = -1;
                    active_clients--;
                } else {
                    buffer[bytes_read - 1] = '\0';
                    printf("Message %d: \"%s\" from %s\n", ++message_counter, buffer, client_names[i]);
                }
            }
        }
    }

    clean_resources(client_count, fifo_fds, fifo_paths);
    printf("All clients disconnected.\n");
    return EXIT_SUCCESS;
}