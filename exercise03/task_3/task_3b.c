#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char *file_path; //file pointer
    int thread_id;   //thread ID
} ThreadArgs;


void *compute_sum(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    FILE *file = fopen(args->file_path, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s: %s\n", args->file_path, strerror(errno));
        return NULL;
    }

    long sum = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *endptr;
        long num = strtol(line, &endptr, 10);
        if (endptr == line || errno == ERANGE) {
            fprintf(stderr, "File passing error %s\n", args->file_path);
            fclose(file);
            return NULL;
        }
        sum += num;
    }

    fclose(file);


    long *result = malloc(sizeof(long));
    if (!result) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }
    *result = sum;
    return (void *)result;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_files = argc - 1;
    pthread_t threads[num_files];
    ThreadArgs thread_args[num_files];
    long *results[num_files];


    for (int i = 0; i < num_files; i++) {
        thread_args[i].file_path = argv[i + 1];
        thread_args[i].thread_id = i + 1;

        int rc = pthread_create(&threads[i], NULL, compute_sum, &thread_args[i]);
        if (rc != 0) {
            fprintf(stderr, "Error creating thread %d: %s\n", i + 1, strerror(rc));
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < num_files; i++) {
        void *result;
        int rc = pthread_join(threads[i], &result);
        if (rc != 0) {
            fprintf(stderr, "Error joining thread %d: %s\n", i + 1, strerror(rc));
            return EXIT_FAILURE;
        }
        results[i] = (long *)result;
    }


    long total_sum = 0;
    for (int i = 0; i < num_files; i++) {
        if (results[i]) {
            printf("sum %d = %ld\n", i + 1, *results[i]);
            total_sum += *results[i];
            free(results[i]);
        }
    }
    printf("total sum = %ld\n", total_sum);

    return EXIT_SUCCESS;
}
