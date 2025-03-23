#define _DEFAULT_SOURCE

#include <signal.h> // for sigaction
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>   
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

//send "signals kill -SIGUSR1 <PID>"
//send "signals kill -SIGUSR2 <PID>"
//send "signals kill -SIGINT <PID>"
//get PID "pgrep signal_handler"

int setupSignalHandler(int signum, void (*handler)(int));

void signalHandler(int signum) {
    //async safe printf, as write is synchronus and does not use buffers which can be inturpetted (e.g. printf bad)
    char msg[50];
    int len = snprintf(msg, sizeof(msg), " Signal received: %d\n", signum);
    write(STDOUT_FILENO, msg, len);

    //modify signal handling based on received signal
    if (signum == SIGUSR1) {
        if (setupSignalHandler(SIGUSR2, SIG_IGN) != EXIT_SUCCESS) {
            perror("Failed to ignore SIGUSR2");
        } else {
            char msg1[30];
            int len1 = snprintf(msg1, sizeof(msg1), "Ignored SIGUSR2 ");
            write(STDOUT_FILENO, msg1, len1);
        }
    } else if (signum == SIGUSR2) {
        if (setupSignalHandler(SIGUSR1, SIG_IGN) != EXIT_SUCCESS) {
            perror("Failed to ignore SIGUSR1");
        }else {
            char msg2[30];
            int len2 = snprintf(msg2, sizeof(msg2), "Ignored SIGUSR1 ");
            write(STDOUT_FILENO, msg2, len2);
        }
    } else if (signum == SIGINT) {
        if (setupSignalHandler(SIGUSR1, signalHandler) != EXIT_SUCCESS ||
            setupSignalHandler(SIGUSR2, signalHandler) != EXIT_SUCCESS) {
            perror("Failed to restore handlers");
        }else {
            char msg3[30];
            int len3 = snprintf(msg3, sizeof(msg3), "Ignored SIGUSR1 ");
            write(STDOUT_FILENO, msg3, len3);
        }
    }
}

    //specifies which action should be taken next time the signal is recieved
    int setupSignalHandler(int signum, void (*handler)(int)) {
    
    //define all other fields, to zero so that masks, flags, etc. are not changed
    struct sigaction act = {0};
    act.sa_handler = handler;

    if (sigaction(signum, &act, NULL) == -1) {
        perror("sigaction failed");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(void) {

        if (setupSignalHandler(SIGINT, signalHandler) != EXIT_SUCCESS) {
            fprintf(stderr, "Failed to register handler for SIGINT\n");
        }
        if (setupSignalHandler(SIGUSR1, signalHandler) != EXIT_SUCCESS) {
            fprintf(stderr, "Failed to register handler for SIGUSR1\n");
        }
        if (setupSignalHandler(SIGUSR2, signalHandler) != EXIT_SUCCESS) {
            fprintf(stderr, "Failed to register handler for SIGUSR2\n");
        }
        if (setupSignalHandler(SIGCONT, signalHandler) != EXIT_SUCCESS) {
            fprintf(stderr, "Failed to register handler for SIGCONT\n");
        }
    
        //SIGSTOP and SIGKILL cannot be caught
        fprintf(stderr, "Warning: SIGSTOP and SIGKILL cannot be caught or handled.\n");

    const time_t work_seconds = 1;

    struct timespec work_duration = {
        .tv_sec = work_seconds,
    };

    struct timespec remaining = {0};

    while (true) {
        // simulate real workload
        if (nanosleep(&work_duration, &remaining) == -1 && errno == EINTR) {
            work_duration = remaining;
            continue;
        }

        // restore work_duration
        work_duration.tv_sec = work_seconds;

        // TODO: more code (only if needed)
    }

    return EXIT_SUCCESS;
}
