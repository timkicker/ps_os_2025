## How do you set/unset environment variables in your shell?

- global:   `export OFFSET=5`

- cmd only: `OFFSET=5 ./arg_counter a b c`

## How can you retrieve the returned exit code in your shell after running the program?

`echo $?` returns the exit code of the last ran program

## In your shell ;, &&, and || can be used to execute multiple commands on a single line. What are the differences between these 3 operators?

- `;`: Runs both commands, one after another, regardless of success/failure
- `&&`: Runs the second command only if the first command succeeds
- `||`: Runs the second command only if the first one fails

## What is the PATH environment variable and why is it important?

Tells your OS where essential programs ("commands") are stored.
Without PATH, you'd have to type full paths for every command. 