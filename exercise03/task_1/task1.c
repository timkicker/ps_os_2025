#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>   
#include <sys/prctl.h> // linux process name

/*
    Debugger may fail, since it stops on signal detection (error "EINTR") per default
    No worries, everything works just fine :))) (i guess)
*/


// prototype
void handle_signal(int signum);

struct sigaction act_handler;
struct sigaction act_ignore;

// set "yeah i want to handle this"
void register_handler(int signum) {
    if (sigaction(signum, &act_handler, NULL) == -1) {
        exit(EXIT_FAILURE);
    }
}

// set signal to "ignore"
void ignore_signal(int signum) {
    if (sigaction(signum, &act_ignore, NULL) == -1) {
        exit(EXIT_FAILURE);
    }
}

void handle_signal(int signum) {
    const char* notify_message = strsignal(signum);
    write(1, notify_message, strlen(notify_message));  
    // 1.. stdout
    // printf is unsafe according to manpage

    switch (signum) {
        case SIGUSR1:
            ignore_signal(SIGUSR2);
            break;
        case SIGUSR2:
            ignore_signal(SIGUSR1);
            break;
        case SIGINT:
            register_handler(SIGUSR1);
            register_handler(SIGUSR2);
            break;
        default:
            break;
    }
}

int main(void) {
    
    // debugging purposes
    // in order to send command later
    // pid for kill and name for pkill
    char name[16];
    pid_t pid = getpid();
    prctl(PR_GET_NAME, (unsigned long)name, 0, 0, 0);
    printf("PID: %d\n",pid);
    printf("Name: %s\n",name);

    // getname is only linux specific
    // replicated from: 
    // https://github.com/avar/test-prctl-name/blob/master/prctl-test.c


    // set up signal handlers
    // memset from: 
    // https://stackoverflow.com/questions/28247501/why-is-my-signal-handler-not-working
    memset(&act_handler, 0, sizeof(act_handler));  // cleanup
    act_handler.sa_handler = handle_signal;
    act_handler.sa_flags = 0;

    memset(&act_ignore, 0, sizeof(act_ignore));
    act_ignore.sa_handler = SIG_IGN;
    act_ignore.sa_flags = 0;


    register_handler(SIGINT);
    register_handler(SIGCONT);
    register_handler(SIGUSR1);
    register_handler(SIGUSR2);
    // SIGKILL and SIGSTOP cannot be handled
    // Source: https://www.youtube.com/watch?v=83M5-NPDeWs

    const time_t work_seconds = 1;

    struct timespec work_duration = {
        .tv_sec = work_seconds,
    };

    struct timespec remaining = {0};

    while (true) {
        // simulate real workload

        // debugger will break here because of "EINTR" signal
        if (nanosleep(&work_duration, &remaining) == -1 && errno == EINTR) {
            work_duration = remaining;
            continue;
        }

        // restore work_duration
        work_duration.tv_sec = work_seconds;

        // TODO: more code (only if needed)
        // nah
    }

    return EXIT_SUCCESS;
}
