#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>

double DR_p(const int t, const int n, const unsigned long long s) {
    unsigned long long h = 0;
    for (unsigned long long i = s; i--;) {
      h += (rand() % n + rand() % n + 2 == t);
    }
    return (double)h / s;
  }


int main(int argc, char *argv[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n> <s>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);  // sides
    int s = atoi(argv[2]);  // dice rolls

    int min_target = 2;
    int max_target = 2 * n;
    int num_children = max_target - min_target + 1;

    // start timer
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_children; i++)
    {
        pid_t pid = fork();  // first child

        if (pid == 0) {
            srand(getpid());
            
            int t = min_target + i; 
            
            double probability = DR_p(t, n, s);
            
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            double elapsed = (now.tv_sec - start.tv_sec) +
                             (now.tv_nsec - start.tv_nsec) / 1e9;
           
            
            printf("Child %d PID = %d. DR_p(%d,%d,%d) = %.6f. Elapsed time = %.6fs\n",
                   i, getpid(), t, n, s, probability, elapsed);
            
            // Child exits after finishing its work
            exit(EXIT_SUCCESS);
        }
    }

    
    for (int i = 0; i < num_children; i++) {
        wait(NULL);
    }

    printf("Done.\n");
    return EXIT_SUCCESS;
    
}

