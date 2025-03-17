#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>


double DR_p(const int t, const int n, const unsigned long long s) {
    unsigned long long h = 0;
    for (unsigned long long i = s; i--;) {
        h += (rand() % n + rand() % n + 2 == t);
    }
    return (double)h / s;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n> <s>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);          //what kind of d it is
    unsigned long long s = strtoull(argv[2], NULL, 10); //sim interations

    if (n < 2 || s < 1) {
        fprintf(stderr, "Invalid input: n must be >= 2 and s must be >= 1\n");
        exit(EXIT_FAILURE);
    }

    //stopwatch
    struct timespec start_time;
    if (clock_gettime(CLOCK_MONOTONIC, &start_time) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    //children with possibilities
    int min_target = 2;
    int max_target = 2 * n;
    int num_children = max_target - min_target + 1;

    for (int i = 0; i < num_children; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            int t = min_target + i; 
            srand(getpid());


            double probability = DR_p(t, n, s);


            struct timespec current_time;
            if (clock_gettime(CLOCK_MONOTONIC, &current_time)) {
                perror("clock_gettime");
                exit(EXIT_FAILURE);
            }
            double elapsed_time = (current_time.tv_sec - start_time.tv_sec) +
                                 (current_time.tv_nsec - start_time.tv_nsec) / 1e9;


            printf("Child %d PID = %d. DR_p(%d,%d,%llu) = %.6f. Elapsed time = %.6fs\n",
                   i, getpid(), t, n, s, probability, elapsed_time);

            exit(EXIT_SUCCESS);
        }
    }


    for (int i = 0; i < num_children; i++) {
        if (wait(NULL) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }


    printf("Done.\n");

    return 0;
}