- What difference do you notice between `SIGINT` and `SIGKILL`?
    - SIGINT: Gives the process at least a chance to clean up his stuff before terminating
    - SIGKILL: aims for the process's head and shoots without asking questions. Cannot be handled by the process itself obviously
- What can the signals `SIGSTOP` and `SIGCONT` be used for?
    - SIGSTOP: Pauses the process
    - SIGCONT: Continues the process
- What happens when you press `CTRL + c` while the program is running (as a foreground process)?
    - Usually sends SIGINT to process
    - Our case: Just prints out "Interrupt" since my programm is handling SIGINT by itself

- What happens when you press `CTRL + z` while the program is running (as a foreground process)?
    - Sends previously discussed SIGSTOP signal.
    - Somehow i cannot continue my program though (?)
- Hypothetically, what would happen in your program if another signal arrives while your signal handler is still executing? Does it make a difference if it is the same signal or a different one?
    - As per linux, the kernel should block the previously received signal for the duration of handling the interrupt. The behaviour of the handler in relation to other signals can be defined via the `sa_mask`

    - All accoring to the dedicated gnu-page: https://www.gnu.org/software/libc/manual/html_node/Signals-in-Handler.html 