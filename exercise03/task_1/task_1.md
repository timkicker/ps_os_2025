
## What difference do you notice between SIGINT and SIGKILL?
- `SIGINT` is a signal that can be caught and handled by a program (e.g., triggered by pressing `CTRL + C`). It allows the program to clean up or perform any other necessary actions before exiting.
- `SIGKILL` is a signal that cannot be caught, blocked, or ignored. It forcefully terminates the program immediately, without giving it the opportunity to clean up.

---

## What can the signals SIGSTOP and SIGCONT be used for?

- `SIGSTOP` pauses (suspends) a process, putting it into a stopped state. It cannot be caught or ignored.
- `SIGCONT` resumes a stopped process, allowing it to continue executing. These signals are commonly used for job control in shells (e.g., pausing and resuming processes).

---

## What happens when you press CTRL + C while the program is running (as a foreground process)?
- Pressing `CTRL + C` sends a `SIGINT` signal to the program. In response, the `signalHandler` function executes, printing "Signal received: 2" (since `SIGINT` has a value of 2). The program will also restore the handlers for `SIGUSR1` and `SIGUSR2`, enabling them to be handled again.

---

## What happens when you press CTRL + Z while the program is running (as a foreground process)?
**Answer!**
- Pressing `CTRL + Z` sends a `SIGTSTP` signal (not `SIGSTOP`), which suspends the process and moves it to the background. The program stops executing until resumed with `SIGCONT` (e.g., using the `fg` command in the shell).

---

## Hypothetically, what would happen in your program if another signal arrives while your signal handler is still executing? Does it make a difference if it is the same signal or a different one?
- If another signal arrives while the signal handler is executing, the behavior depends on the signal and the `sa_mask` settings in `struct sigaction`. In this program, `sigemptyset(&act.sa_mask)` is used, meaning no signals are blocked during the handler's execution.
  - If the same signal arrives, it may re-enter the handler, potentially leading to undefined behavior or a race condition.
  - If a different signal arrives, its handler will execute immediately, possibly interrupting the current handler.
- To prevent this, block relevant signals during the handler’s execution by using `sigaddset(&act.sa_mask, ...)` in the `setup_signal_handler` function. This ensures that the handler is not interrupted by the same or other specified signals.
