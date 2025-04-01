#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/poll.h>

void clean(int range, int *fifo_file_descriptors, char fifo_paths[][1024]) {
        for (int i = 0; i < range; i++) {
                close(fifo_file_descriptors[i]);
                unlink(fifo_paths[i]);
        }
}

int main(int argc, char **argv) {
        if (argc < 2) {
                fprintf(stderr, "Usage: server.c <client1> ... <clientN>\n");
                return EXIT_FAILURE;
        }

        int potential_clients = argc - 1;
        int connected_clients = 0;
        char fifo_paths[potential_clients][1024];
        char client_names[potential_clients][128];
        int fifo_file_descriptors[potential_clients];
        struct pollfd poll_file_descriptors[potential_clients];
        int message_count = 0;

        for (int i = 0; i < potential_clients; ++i) {
                snprintf(client_names[i], sizeof(client_names[i]), "%s", argv[i + 1]);
                snprintf(fifo_paths[i], sizeof(fifo_paths[i]), "/tmp/%s_csbb8841_fifo", client_names[i]);

                if (mkfifo(fifo_paths[i], 0666) == -1) {
                        fprintf(stderr, "error creating fifo\n");
                        clean(potential_clients, fifo_file_descriptors, fifo_paths);
                        return EXIT_FAILURE;
                }

                fifo_file_descriptors[i] = open(fifo_paths[i], O_RDONLY);
                if (fifo_file_descriptors[i] == -1) {
                        fprintf(stderr, "error opening fifo\n");
                        clean(potential_clients, fifo_file_descriptors, fifo_paths);
                        return EXIT_FAILURE;
                }

                poll_file_descriptors[i].fd = fifo_file_descriptors[i];
                poll_file_descriptors[i].events = POLLIN;
                printf("%s connected.\n", client_names[i]);
                connected_clients++;
        }

        while (connected_clients) {
                int poll_count = poll(poll_file_descriptors, potential_clients, -1);
                if (poll_count == -1) {
                        fprintf(stderr, "error with poll\n");
                        clean(potential_clients, fifo_file_descriptors, fifo_paths);
                        return EXIT_FAILURE;
                }

                for (int i = 0; i < potential_clients; ++i) {
                        if (poll_file_descriptors[i].revents & POLLIN) {
                                char message_buffer[1024];
                                ssize_t char_count = read(fifo_file_descriptors[i], message_buffer, sizeof(message_buffer) - 1);

                                if (char_count == -1) {
                                        fprintf(stderr, "error reading from fifo\n");
                                        return EXIT_FAILURE;
                                } else if (char_count == 1) {
                                        printf("%s disconnected.\n", client_names[i]);
                                        close(fifo_file_descriptors[i]);
                                        unlink(fifo_paths[i]);
                                        poll_file_descriptors[i].fd = -1;
                                        connected_clients--;
                                } else {
                                        message_buffer[char_count - 1] = '\0';
                                        message_count++;
                                        printf("Message %d: \"%s\" from %s\n", message_count, message_buffer, client_names[i]);
                                }
                        }
                }


        }
        clean(potential_clients, fifo_file_descriptors, fifo_paths);
        printf("All clients disconnected\n");
        return EXIT_SUCCESS;
}