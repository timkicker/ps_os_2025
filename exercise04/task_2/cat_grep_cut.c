#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// same as first task BUT has 2 pipes

int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int pipe1[2], pipe2[2];
	// src.: https://stackoverflow.com/questions/8389033/implementing-pipes-in-c

	// cat -> grep
	if (pipe(pipe1) == -1) {
		perror("pipe1");
		exit(EXIT_FAILURE);
	}

	// grep -> cut
	if (pipe(pipe2) == -1) {
		perror("pipe2");
		exit(EXIT_FAILURE);
	}

	// cat file1 file2
	pid_t pid_cat = fork();
	if (pid_cat < 0) {
		perror("fork for cat");
		exit(EXIT_FAILURE);
	}
	if (pid_cat == 0) {
		close(pipe1[0]);

		// src.: https://stackoverflow.com/questions/5291593/using-dup2-to-redirect-stdout-in-c
		if (dup2(pipe1[1], STDOUT_FILENO) == -1) {
			perror("dup2 cat");
			exit(EXIT_FAILURE);
		}

		close(pipe1[1]);

		// 2nd pipe not needed
		close(pipe2[0]);
		close(pipe2[1]);

		execlp("cat", "cat", argv[1], argv[2], (char*)NULL);
		perror("execlp cat");
		exit(EXIT_FAILURE);
	}

	// grep '^;'
	pid_t pid_grep = fork();
	if (pid_grep < 0) {
		perror("fork for grep");
		exit(EXIT_FAILURE);
	}
	if (pid_grep == 0) {

		close(pipe1[1]);

		if (dup2(pipe1[0], STDIN_FILENO) == -1) {
			perror("dup2 grep (stdin)");
			exit(EXIT_FAILURE);
		}

		close(pipe1[0]);

		if (dup2(pipe2[1], STDOUT_FILENO) == -1) {
			perror("dup2 grep (stdout)");
			exit(EXIT_FAILURE);
		}

		close(pipe2[1]);

		execlp("grep", "grep", "^;", (char*)NULL);
		perror("execlp grep");
		exit(EXIT_FAILURE);
	}

	// cut -c 22-
	pid_t pid_cut = fork();
	if (pid_cut < 0) {
		perror("fork for cut");
		exit(EXIT_FAILURE);
	}
	if (pid_cut == 0) {

		close(pipe2[1]);

		if (dup2(pipe2[0], STDIN_FILENO) == -1) {
			perror("dup2 cut");
			exit(EXIT_FAILURE);
		}

		close(pipe2[0]);

		// not needed
		close(pipe1[0]);
		close(pipe1[1]);

		execlp("cut", "cut", "-c", "22-", (char*)NULL);
		perror("execlp cut");
		exit(EXIT_FAILURE);
	}

	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[0]);
	close(pipe2[1]);

	waitpid(pid_cat, NULL, 0);
	waitpid(pid_grep, NULL, 0);
	waitpid(pid_cut, NULL, 0);

	return EXIT_SUCCESS;
}
