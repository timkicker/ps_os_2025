#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int64_t accumulation = 0;

void *thread_accumulate(void *arg) {
    long n = *(long *)arg;
    for (long i = 1; i <= n; i++) {
        accumulation += i;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr;
    errno = 0;
    long n = strtol(argv[1], &endptr, 10);
    if (errno != 0 || *endptr != '\0' || n <= 0) {
        fprintf(stderr, "Invalid value for N. Must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    printf("Initial accumulation: %" PRId64 "\n", accumulation);

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1)//error 
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) { // Child process
        for (long i = 1; i <= n; i++) {
            accumulation += i;
        }//used exit to cleanly kill all child processes
        exit(EXIT_SUCCESS);

    } else { // Parent process
        // Wait for child
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return EXIT_FAILURE;
        }
        printf("After child process: %" PRId64 "\n", accumulation);
    }

    // Create a thread
    pthread_t thread;
    int rc = pthread_create(&thread, NULL, thread_accumulate, &n);
    if (rc != 0) {
        fprintf(stderr, "pthread_create: %s\n", strerror(rc));
        return EXIT_FAILURE;
    }

    // Wait for the thread to finish
    rc = pthread_join(thread, NULL);
    if (rc != 0) {
        fprintf(stderr, "pthread_join: %s\n", strerror(rc));
        return EXIT_FAILURE;
    }

    printf("After thread: %" PRId64 "\n", accumulation);

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

