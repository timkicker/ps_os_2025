#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define FIFO_DIR "/tmp/"
#define PIPE_BUF 4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <client_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char fifo_path[100];
    snprintf(fifo_path, sizeof(fifo_path), FIFO_DIR "%s_fifo", argv[1]);

    //https://stackoverflow.com/questions/52904971/opening-a-fifo-for-reading-in-c
    int fd = open(fifo_path, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    printf("Message: \n");
    char buffer[PIPE_BUF];

    while (1) {
        if (fgets(buffer, PIPE_BUF, stdin) == NULL) {
            break;
        }

        size_t len = strlen(buffer);
        if (buffer[len - 1] == '\n') { //removing newline character
            buffer[len - 1] = '\0';
        }

        if (strlen(buffer) == 0) { //if empty message, disconnect
            break;
        }

        write(fd, buffer, strlen(buffer));
        printf("Message: \n");
    }

    close(fd);
    return EXIT_SUCCESS;
}
