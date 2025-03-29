#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>
#include <errno.h>

#define FIFO_DIR "/tmp/"
#define MAX_CLIENTS 2
#define PIPE_BUF 4096

typedef struct
{
    char name[50];
    char fifo_path[100];
    int fd;
} Client;

void cleanup_fifos(Client *clients, int num_clients)
{
    for (int i = 0; i < num_clients; i++)
    {
        close(clients[i].fd);         // closing file descriptor
        unlink(clients[i].fifo_path); // removing fifo file
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <client1> <client2> ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_clients = argc - 1;
    if (num_clients > MAX_CLIENTS)
    {
        fprintf(stderr, "Too many clients. Max is %d.\n", MAX_CLIENTS);
        return EXIT_FAILURE;
    }

    Client clients[MAX_CLIENTS];
    struct pollfd poll_fds[MAX_CLIENTS];
    int message_count = 0;

    for (int i = 0; i < num_clients; i++)
    {
        snprintf(clients[i].name, sizeof(clients[i].name), "%s", argv[i + 1]);

        // create a temporary buffer to avoid any overlap
        char temp_fifo_path[100];

        // create the FIFO path string
        snprintf(temp_fifo_path, sizeof(temp_fifo_path), FIFO_DIR "%s_fifo", clients[i].name);

        // now assign the generated path to the client's fifo_path
        strncpy(clients[i].fifo_path, temp_fifo_path, sizeof(clients[i].fifo_path) - 1);
        clients[i].fifo_path[sizeof(clients[i].fifo_path) - 1] = '\0'; // ensure null-termination

        // https://stackoverflow.com/questions/3790921/creating-named-pipes-in-c
        if (mkfifo(clients[i].fifo_path, 0666) == -1 && errno != EEXIST)
        {
            perror("mkfifo");
            cleanup_fifos(clients, i);
            return EXIT_FAILURE;
        }

        // https://stackoverflow.com/questions/52904971/opening-a-fifo-for-reading-in-c
        clients[i].fd = open(clients[i].fifo_path, O_RDONLY);
        if (clients[i].fd == -1)
        {
            perror("open");
            cleanup_fifos(clients, i);
            return EXIT_FAILURE;
        }

        poll_fds[i].fd = clients[i].fd;
        poll_fds[i].events = POLLIN;

        printf("%s connected.\n", clients[i].name);
    }

    while (num_clients > 0)
    {
        // https://stackoverflow.com/questions/70467701/how-to-use-poll-in-c
        int ret = poll(poll_fds, num_clients, -1);
        if (ret == -1)
        {
            perror("poll");
            cleanup_fifos(clients, num_clients);
            return EXIT_FAILURE;
        }

        for (int i = 0; i < num_clients; i++)
        {

            // set a small timeout for poll (100ms), so that client termiantes
            int ret = poll(poll_fds, num_clients, 100);
            if (ret == -1)
            {
                perror("poll");
                cleanup_fifos(clients, num_clients);
                return EXIT_FAILURE;
            }
            if (poll_fds[i].revents & POLLIN)
            {
                char buffer[PIPE_BUF];
                ssize_t bytes_read = read(clients[i].fd, buffer, PIPE_BUF - 1); // reading message

                if (bytes_read > 0)
                {
                    buffer[bytes_read] = '\0';
                    printf("Message %d: \"%s\" from %s\n", ++message_count, buffer, clients[i].name);
                }
                else
                {
                    printf("%s disconnected.\n", clients[i].name);
                    close(clients[i].fd);
                    unlink(clients[i].fifo_path);

                    for (int j = i; j < num_clients - 1; j++)
                    {
                        clients[j] = clients[j + 1];
                        poll_fds[j] = poll_fds[j + 1];
                    }
                    num_clients--;
                    i--; // adjust index since clients shifted
                }
            }
        }
        if (num_clients == 0) {
            break; //doesnt seem to end, perhaps I need to remove the ln linebreak to make the message truly empty
    }
    }
    printf("All clients disconnected.\n");
    return EXIT_SUCCESS;
}
