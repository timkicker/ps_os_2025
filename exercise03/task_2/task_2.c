#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void string_shift_right(int arg_index, char* input, const char* original) {
	size_t input_length = strlen(input);
	//added +1 to include the null-terminator
	//char shifted_input[input_length];
	char shifted_input[input_length+1];

	for (size_t char_index = 0; char_index < input_length; ++char_index) {
		//out of bounds safe guard by using modulo and having it wrap around again
		//size_t new_position = (char_index + 2) / input_length;
		size_t new_position = (char_index + 2) % input_length;
		shifted_input[new_position] = input[char_index];
	}

	//ensure the null-terminator exists at the end of the shifted array
	shifted_input[input_length] = '\0';


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
		
		//again add 1 for null-terminator, otherwise strcpy would try and copy strings into buffers which are too small
		//shared_strings[arg_index] = malloc(arg_length * sizeof(char));
		shared_strings[arg_index] = malloc((arg_length+1) * sizeof(char));
		if (shared_strings[arg_index] == NULL) {
			fprintf(stderr, "Could not allocate memory.\n");
			exit(EXIT_FAILURE);
		}
		//strcpy is trying to copy a string from argv[arg_index] to the destination, however this is too small, fixed by line 13
		//strncpy(shared_strings[arg_index], argv[arg_index], arg_length + 1) would be safer as it knows how long to copy
		strcpy(shared_strings[arg_index], argv[arg_index]);
		
	}

	//use < because otherwise, argv[argc] = NULL resulting in an error
	//for (int arg_index = 0; arg_index <= argc; arg_index++) {
	for (int arg_index = 0; arg_index < argc; arg_index++) {
		pid_t pid = fork();
		if (pid == -1) {
			perror("Could not create fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			string_shift_right(arg_index, shared_strings[arg_index], argv[arg_index]);
			//forgot to free memory used by child process
            for (int i = 0; i < argc; i++) {
                free(shared_strings[i]);
            }
			exit(EXIT_SUCCESS);
		}
	}



	//wrong usage of pthread_join, as pthread_join is for threads and not process
	for (int arg_index = 0; arg_index <= argc; arg_index++) {
		//pthread_join(arg_index, NULL);
		waitpid(-1, NULL, 0); //wait for any child process

	}

	//no free command, meaning memory is not freed after parent usage and therefore lost
	for (int arg_index = 0; arg_index < argc; arg_index++) {
		free(shared_strings[arg_index]);
	}
	printf("Done.");
	return EXIT_SUCCESS;
}
