### How do you set/unset environment variables in your shell?

To set an environment variable in the shell, use the `export` command:
```bash
export MY_VAR=value
```
To unset it, simply use `unset`:
```bash
unset MY_VAR
```

### How can you retrieve the returned *exit code* in your shell after running the program?

You can get the exit code of the last command using the `$?` variable:
```bash
./my_program
echo $?
```
This will print the exit code of the last executed program.

### In your shell `;`, `&&`, and `||` can be used to execute multiple commands on a single line. What are the differences between these 3 operators?

- `;` lets you run multiple commands in sequence, no matter if the previous one succeeded or not:
  ```bash
  command1; command2
  ```
  
- `&&` runs the second command only if the first one succeeds (exit code 0):
  ```bash
  command1 && command2
  ```
  This way, `command2` only runs if `command1` completes without errors.
  
- `||` runs the second command only if the first one fails (non-zero exit code):
  ```bash
  command1 || command2
  ```
  In this case, `command2` runs if `command1` encounters an error.

### What is the `PATH` environment variable and why is it important?

The `PATH` variable contains a list of directories the shell searches for executables when you type a command. It allows you to run commands without specifying their full path. The `PATH` is important because it tells the shell where to find the programs you're trying to execute. Without the correct directories in your `PATH`, you’d need to provide the full path every time you run a program.
