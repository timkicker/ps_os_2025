#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipe_fd[2];
    // https://stackoverflow.com/questions/8389033/implementing-pipes-in-c
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(pipe_fd[0]);
        // https://stackoverflow.com/questions/5291593/using-dup2-to-redirect-stdout-in-c
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        execlp("cat", "cat", argv[1], argv[2], (char *)NULL);
        perror("execlp cat");
        exit(EXIT_FAILURE);
    }

    close(pipe_fd[1]);
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[0]);

    // https://stackoverflow.com/questions/2605130/redirection-in-exec
    execlp("cut", "cut", "-c", "22-", (char *)NULL);
    perror("execlp cut");
    exit(EXIT_FAILURE);
}
