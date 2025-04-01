#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: Clang format & delete binaries!!

// only for multiple arguments
typedef struct {
	int thread_id;
	const char* filename;
} thread_args;

void* thread_sum(void* arg) {

	thread_args* args = (thread_args*)arg;

	int64_t* result = malloc(sizeof(int64_t));
	if (result == NULL) {
		perror("malloc");
		pthread_exit(NULL);
	}
	*result = 0;

	FILE* fp = fopen(args->filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file '%s': %s\n", args->filename, strerror(errno));
		return result;
	}

	int64_t sum = 0;
	int number;

	while (fscanf(fp, "%d", &number) == 1) {
		sum += number;
	}
	fclose(fp);

	*result = sum;
	return result;
}

int main(int argc, char* argv[]) {

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <file1> <file2> ...\n", argv[0]);
		return EXIT_FAILURE;
	}

	int N = argc - 1; // files

	pthread_t* threads = malloc(N * sizeof(pthread_t));
	if (threads == NULL) {
		perror("malloc");
		return EXIT_FAILURE;
	}

	thread_args* args_array = malloc(N * sizeof(thread_args));
	if (args_array == NULL) {
		perror("malloc");
		free(threads);
		return EXIT_FAILURE;
	}

	// create thr
	for (int i = 0; i < N; i++) {
		args_array[i].thread_id = i + 1;
		args_array[i].filename = argv[i + 1];

		int ret = pthread_create(&threads[i], NULL, thread_sum, &args_array[i]);
		if (ret != 0) {
			fprintf(stderr, "pthread_create failed for thread\n");
			free(threads);
			free(args_array);
			return EXIT_FAILURE;
		}
	}

	int64_t* results = malloc(N * sizeof(int64_t));
	if (results == NULL) {
		perror("malloc");
		free(threads);
		free(args_array);
		return EXIT_FAILURE;
	}

	// join
	// allocate result with value in array (important for order)
	for (int i = 0; i < N; i++) {

		void* thread_result;

		int ret = pthread_join(threads[i], &thread_result);
		if (ret != 0) {
			fprintf(stderr, "join fail");
			free(threads);
			free(args_array);
			free(results);
			return EXIT_FAILURE;
		}

		results[i] = *(int64_t*)thread_result;
		free(thread_result);
	}

	int64_t total_sum = 0;
	for (int i = 0; i < N; i++) {
		printf("sum %d = %ld\n", i + 1, results[i]);
		total_sum += results[i];
	}
	printf("total sum = %ld\n", total_sum);

	free(threads);
	free(args_array);
	free(results);

	return EXIT_SUCCESS;
}
