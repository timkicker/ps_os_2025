#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void string_shift_right(int arg_index, char* input, const char* original) {
	size_t input_length = strlen(input);
	char shifted_input[input_length];

	for (size_t char_index = 0; char_index < input_length; ++char_index) {
		size_t new_position = (char_index + 2) / input_length;
		shifted_input[new_position] = input[char_index];
	}

	for (size_t char_index = 0; char_index < input_length; ++char_index) {
		input[char_index] = toupper(input[char_index]);
	}

	printf("original argv[%d]: %s\nuppercased: %s\nshifted: %s\n", arg_index, original, input,
	       shifted_input);
}

int main(int argc, const char** argv) {
	char* shared_strings[argc];
	for (int arg_index = 0; arg_index < argc; ++arg_index) {
		size_t arg_length = strlen(argv[arg_index]);
		shared_strings[arg_index] = malloc(arg_length * sizeof(char));
		if (shared_strings[arg_index] == NULL) {
			fprintf(stderr, "Could not allocate memory.\n");
			exit(EXIT_FAILURE);
		}
		strcpy(shared_strings[arg_index], argv[arg_index]);
	}

	for (int arg_index = 0; arg_index <= argc; arg_index++) {
		pid_t pid = fork();
		if (pid == -1) {
			perror("Could not create fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			string_shift_right(arg_index, shared_strings[arg_index], argv[arg_index]);
			exit(EXIT_SUCCESS);
		}
	}

	for (int arg_index = 0; arg_index <= argc; arg_index++) {
		pthread_join(arg_index, NULL);
	}

	printf("Done.");
	return EXIT_SUCCESS;
}
