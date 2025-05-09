#include <errno.h>
#include <pthread.h>
#include <stdint.h> // int64
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strerror
#include <sys/wait.h>
#include <unistd.h>

/*
    Debug:
    Comment line 29 till 39 for debug without passing arguments
    And uncomment 42


    "How is it that fork does not change the acc but a new thread does?"
    A: a fork creates a seperate process with its own memory, whose are not visible
       by the parent process. Threads however share their memory, they only got their
       own stack.

*/

int64_t accumulation = 0;

void* accumulate(void* arg) {
	int n = *(int*)arg;
	for (int i = 1; i <= n; i++) {
		accumulation += i;
	}
	return NULL;
}

int main(int argc, const char** argv) {

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <N>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int n = atoi(argv[1]);
	if (n <= 0) {
		fprintf(stderr, "N must be a positive integer.\n");
		return EXIT_FAILURE;
	}

	// int n = 5;   // only for debug

	printf("Before fork: accumulation = %ld\n", accumulation);

	pid_t pid = fork();

	if (pid == -1) {
		perror("fork failed");
		return EXIT_FAILURE;
	}

	if (pid == 0) {
		for (int i = 1; i <= n; i++) {
			accumulation += i;
		}

		// found out that this is the more commonly used way than "return"
		// dont know why though but it's considered best practise
		exit(EXIT_SUCCESS);
	} else {
		wait(NULL);
		printf("After fork: accumulation = %ld\n", accumulation);
	}

	printf("Before thread: accumulation = %ld\n", accumulation);

	pthread_t thread;
	int result = pthread_create(&thread, NULL, accumulate, &n);
	if (result != 0) {
		fprintf(stderr, "pthread_create failed: %s\n", strerror(result));
		return EXIT_FAILURE;
	}

	result = pthread_join(thread, NULL);
	if (result != 0) {
		fprintf(stderr, "pthread_join failed: %s\n", strerror(result));
		return EXIT_FAILURE;
	}

	printf("After thread: accumulation = %ld\n", accumulation);

	return EXIT_SUCCESS;
}

/*
processes don’t share memory, so when the child updates accumulation,
it only changes in the child’s memory space, and the parent doesn’t see the update.

threads share memory, meaning the thread’s changes to accumulation
directly affect the global variable in the main thread.

possible race condition: since accumulation is shared, if multiple threads
were added without synchronization, they could interfere with each other’s updates.

this highlights a key difference:
child processes have isolated memory,
while threads operate in a shared memory space.
*/