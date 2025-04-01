## Shell Redirection Operators

### `<` (Input Redirection)
Redirects a file’s content as input to a command.
Example:
```sh
sort < file.txt
```
Sorts `file.txt` using `sort`.

### `>` (Output Redirection)
Redirects standard output to a file (overwrites if it exists).
Example:
```sh
echo "Hello" > output.txt
```
Writes "Hello" to `output.txt`.

### `2>` (Standard Error Redirection)
Redirects error messages (stderr) to a file.
Example:
```sh
ls nonexistentfile 2> errors.txt
```
Saves the error message to `errors.txt`.

### `2>&1` (Merge Stdout and Stderr)
Redirects stderr (`2>`) to stdout (`1>`), combining them.
Example:
```sh
ls nonexistentfile > output.txt 2>&1
```
Saves both standard output and errors in `output.txt`.
