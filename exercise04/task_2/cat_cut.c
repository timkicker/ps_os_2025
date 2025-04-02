#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
		exit(EXIT_FAILURE); // good practise instead of return
		                    // src: https://www.geeksforgeeks.org/c-exit-abort-and-assert-functions/
	}

	int pipeflgs[2];
	if (pipe(pipeflgs) == -1) {
		perror("pipe"); // prints error (src: manpage)
		exit(EXIT_FAILURE);
	}

	pid_t pid_cat = fork();
	if (pid_cat < 0) {
		perror("fork pid_cat");
		exit(EXIT_FAILURE);
	}
	if (pid_cat == 0) {
		close(pipeflgs[0]);

		// redirect
		if (dup2(pipeflgs[1], STDOUT_FILENO) == -1) {
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pipeflgs[1]);

		// l... list of arguments
		// p... search in path
		// ends with nullp (char*)NULL
		// src.: https://stackoverflow.com/questions/2605130/redirection-in-exec
		execlp("cat", "cat", argv[1], argv[2], (char*)NULL);

		// process replaced now

		// if not
		perror("execlp cat");
		exit(EXIT_FAILURE);
	}

	pid_t pid_cut = fork();
	if (pid_cut < 0) {
		perror("fork pid_cut");
		exit(EXIT_FAILURE);
	}
	if (pid_cut == 0) {
		close(pipeflgs[1]);

		// stdin to read
		if (dup2(pipeflgs[0], STDIN_FILENO) == -1) {
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pipeflgs[0]);
		execlp("cut", "cut", "-c", "22-", (char*)NULL);

		perror("execlp cut");
		exit(EXIT_FAILURE);
	}

	close(pipeflgs[0]);
	close(pipeflgs[1]);

	waitpid(pid_cat, NULL, 0);
	waitpid(pid_cut, NULL, 0);
}