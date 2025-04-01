#include <ctype.h>
// #include <pthread.h>		view line 65
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Still unclear: Why are we mixing threads and processes???
// Intended error?

void string_shift_right(int arg_index, char* input, const char* original) {
	size_t input_length = strlen(input);

	// strlen does not take 0-terminator into account
	// char shifted_input[input_length];
	char shifted_input[input_length + 1];
	shifted_input[input_length] = '\0';

	for (size_t char_index = 0; char_index < input_length; ++char_index) {
		// since we do want to use a circular approach, % is needed
		//  size_t new_position = (char_index + 2) / input_length;
		size_t new_position = (char_index + 2) % input_length;
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
		// strlengh does not take '\0' into account
		// size_t arg_length = strlen(argv[arg_index]);
		size_t arg_length = strlen(argv[arg_index]) + 1;
		shared_strings[arg_index] = malloc(arg_length * sizeof(char));
		if (shared_strings[arg_index] == NULL) {
			fprintf(stderr, "Could not allocate memory.\n");
			exit(EXIT_FAILURE);
		}
		strcpy(shared_strings[arg_index], argv[arg_index]);
	}

	// "<=" creates an extra iteration, wich results in "out of bounce"
	// for (int arg_index = 0; arg_index <= argc; arg_index++) {
	for (int arg_index = 0; arg_index < argc; arg_index++) {
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

	// same itearation error as for-loop above
	// for (int arg_index = 0; arg_index <= argc; arg_index++) {
	for (int arg_index = 0; arg_index < argc; arg_index++) {
		// pthread is for threads, wait is for forking processes
		// pthread_join(arg_index, NULL);
		wait(NULL);
	}

	// free the allocated shared_strings!!!
	for (int arg_index = 0; arg_index < argc; arg_index++) {
		free(shared_strings[arg_index]);
	}

	printf("Done.");
	return EXIT_SUCCESS;
}
