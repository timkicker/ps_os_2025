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

    int pipe1[2], pipe2[2];
    // https://stackoverflow.com/questions/8389033/implementing-pipes-in-c
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        close(pipe1[0]);
        // https://stackoverflow.com/questions/5291593/using-dup2-to-redirect-stdout-in-c
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[1]);
        execlp("cat", "cat", argv[1], argv[2], (char *)NULL);
        perror("execlp cat");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        close(pipe1[1]);
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);
        close(pipe2[0]);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[1]);
        execlp("grep", "grep", "^;", (char *)NULL);
        perror("execlp grep");
        exit(EXIT_FAILURE);
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[1]);
    
    dup2(pipe2[0], STDIN_FILENO);
    close(pipe2[0]);
    
    // https://stackoverflow.com/questions/2605130/redirection-in-exec
    execlp("cut", "cut", "-c", "22-", (char *)NULL);
    perror("execlp cut");
    exit(EXIT_FAILURE);
}
