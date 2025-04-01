#define _POSIX_C_SOURCE 200809L // tell the compiler i want to use POSIX.1-2008 (used for sigact)

#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum Exit_Code {
	NOARGS = 13,
	OVER10ARGS = 7,
	INTOFFSET = 99,
	NOOFFSET = 43,
	INTEXIT = 42
};

void handle(int signum);

void handle(int signum) {
	fprintf(stderr, "Interrupted with signal %d\n", signum);
	exit(INTEXIT);
}

int main(int argc, const char* argv) {

	// set env
	// global:   export OFFSET=5
	// cmd only: OFFSET=5 ./arg_counter a b c
	//      output = "Result 8"

	const char* offset_str = getenv("OFFSET");
	int offset = 0;

	if (offset_str != NULL) {
		char* endptr;
		offset = strtol(offset_str, &endptr, 10);
		if (*endptr != '\0') {
			return INTOFFSET;
		}
	} else {
		return NOOFFSET;
	}

	int user_args = argc - 1;
	int total = user_args + offset;

	if (user_args == 0) {
		fprintf(stderr, "Usage: %s <arg1> [arg2 ... arg10]\n", argv[0]);
		return NOARGS;
	}

	if (user_args > 10) {
		return OVER10ARGS;
	}

	printf("Result: %d\n", total);
	return EXIT_SUCCESS;
}
