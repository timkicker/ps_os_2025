# Exercise Sheet 1

This exercise sheet contains the self-assessment exercise along with some introductory exercises to using the command-line.

## Task 1

This first task does not require the use of any operating systems concepts besides what you have already learned in the previous semester.
Instead, the purpose of this task is to check (for yourself) whether you are sufficiently fluent in using the C programming language.
Most exercises in this course will build directly on top of what you've learned in the previous semester.
If you are struggling with this task, consider doing this course a year later, familiarizing yourself with the C Programming language in the meantime.
The workload might otherwise be higher for you than originally planned by us.

Your task is to program [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) using the C programming language.
To visualize the simulation, you have to generate a [PBM image](https://en.wikipedia.org/wiki/Netpbm#PBM_example) after every simulation step.
These images can then be combined into an animated gif using [ImageMagick](https://imagemagick.org/index.php).
ImageMagick can be used via the `convert` command-line program (see below).

A starting [template](task1/template/) is provided, but you may start from scratch.

Your program does the following:

- Read in the following parameters from the command-line (see template)
  - Width of the game field (integer)
  - Height of the game field (integer)
  - Starting population density (float [0-1])
  - Number of simulation steps (integer)

- Seed the random number generator so we get a different starting population every time (see template)

- **Dynamically allocate** 2 game fields according to the given *width* and *height* (use `malloc`)
  - A game field is a big array of booleans where each element indicates whether the cell at this position (`x`, `y`) is alive or dead
  - For each simulation step one field will be the *source* while the other is the *destination*
  - Source and destination alternate every simulation step

- According to the given population density, mark cells in one field as alive (see `rand` standard library function)
  - A population density of 0.1 means that around 10% of all cells are marked alive
  - This field will be the source for the first simulation step

- Generate the first PBM image to record the starting population (use `fopen`, `fprintf`, `fclose`, …)
     - Each file should be named `gol_<step>.pbm` where `<step>` is the corresponding simulation step padded with zeros up to 5 digits.
       For example, the image for step 5 would be `gol_00005.pbm`.

- Run the simulation for the given number of steps
  - After every simulation step, generate a PBM file to record the result (see `snprintf` to generate the filename)

- Don't forget to clean up (use `free`)

- Your program must compile without warnings using the following flags:
  - `-std=c11 -Wall -Wextra`
  - Either compile your program with [AddressSanitizer](https://en.wikipedia.org/wiki/AddressSanitizer) enabled (`-fsanitize=address`) or run it via `valgrind` to check for errors

Here is how one would use the program (here named `game`) and then generate the animated gif.

```
$ ./game 200 200 0.1 10
width:    200
height:   200
density:   10%
steps:     10

$ ls *.pbm        # Here we see the generated PBM images, lets call ImageMagick next
gol_00000.pbm
gol_00001.pbm
gol_00002.pbm
gol_00003.pbm
gol_00004.pbm
gol_00005.pbm
gol_00006.pbm
gol_00007.pbm
gol_00008.pbm
gol_00009.pbm
gol_00010.pbm

$ convert -filter point -resize 300%x300% -delay 20 gol_*.pbm gol.gif
```

![Example](task1/example.gif)

## Task 2

As a computer scientist you will likely spend a lot of your time in a terminal.
It is therefore a good idea to get accustomed to a few basic commands.
We will gradually introduce new commands and concepts over the semester.

For now, you should teach yourself how to do the following tasks.
Introductory [slides from the previous year](../lab01/1_history_and_shell.pdf) are provided, but you are free to use any tutorial you find.

- Connecting to ZID-GPL via `ssh`
  - See [LPCCS System](https://www.uibk.ac.at/zid/systeme/linux/)
  - If the `ssh` command is not available on your system, install the *OpenSSH Client*
    - Windows: Settings > Apps & features > Optional features > OpenSSH Client
    - Ubuntu: `sudo apt install openssh-client` in your terminal
    - MacOS: `brew install openssh` (https://brew.sh/) 
  - You can close the connection by logging out with the `exit` command

- Looking around, changing directory (`pwd`, `ls`, `cd`)
- Creating files and directories (`touch`, `mkdir`)
- Copying, moving, deleting files and directories (`cp`, `mv`, `rm`)
- Filtering an input (`grep`)
- Redirect output of a command `>` (e.g. `ls > text.txt`)
- Chaining commands with a pipe `|` (e.g. `ls | grep "txt"`)
- Using a terminal text editor (`vim`)
  - How to edit and save a document? How to exit the editor?
- Viewing history of previously used commands (`history`)
- Print content of a file (`cat`)
- Getting help
  - `man` to view the manual (man) page of a command
  - `help` to get information on shell built-ins (e.g. `help cd`)
    Try this if there is no man page available
  - `info` to view the info page of a command
    This can sometimes give you more information than the man page
  - `apropos` to search for man pages
  - Appending `--help` to a command (e.g. `ls --help`)

Next, read the following man pages to get a better understanding of your surroundings:

- Read `man intro`
- Skim over `man man-pages`, take note of the sections
- Skim over `man file-hierarchy`
- Figure out the difference between `man printf` and `man 3 printf`

Finally:

- Navigate to your home folder (`~`), create a new folder, create a new file in that folder
- List the content of your homefolder and redirect the output to your newly created file
- Figure out a way to copy the file from ZID-GPL to your local system and vice versa
- How can you retrace these steps in the future?
  - How can you search for previously used commands?

Put together a summary of all the commands and man pages mentioned above.
- Write the summary on ZID-GPL using `vim`

## Task 3

The program [`list_args.c`](task3/list_args.c) prints its commandline arguments when run, one per line. Use `make` to build the executable `list_args`.
Figure out how to run the program to produce the following output:

```
0: ./list_args
1: hi
2: hello world
3: "linux"
4: 'ps' "os"
5: its week one \ or is it?
```
- Write the command you used to produce this output to a file called `args.txt`.
- Finally, use a single-line `bash` command to read the program call from `args.txt` and execute it. Provide the used command in a new file `run.txt`. Have a look at the `eval(1)` man page.

After creating both files (`args.txt` and `run.txt`), use `make handin` to generate a file `task_3.txt` and verify that it contains your solutions.

--------------

Submit your solution as a zip archive via OLAT, structured as follows, where csXXXXXX is your UIBK login name. Your zip archive **must not** contain binaries or additional files. Any implementation **must not** produce any additional output.

```text
exc01_csXXXXXX.zip
├── group.txt            # optional
├── task_1
│   ├── Makefile         # optional
│   └── game.c
├── task_2.txt           # or .md
└── task_3.txt
```

Requirements

- [ ] Auto-format all source files
- [ ] Check your submission on ZID-GPL
- [ ] Check your file structure
- [ ] Submit zip
- [ ] Mark solved exercises in OLAT
- [ ] Any implementation MUST NOT produce any additional output
- [ ] If you work in a group, create a `group.txt` file according to the format specified below.

If you worked in a group, the `group.txt` file must be present
and have one line per student which contains the matriculation number
in the beginning, followed by a space and the student's name.
For example, if the group consists of Jane Doe,
who has matriculation number 12345678,
and Max Mustermann, who has matriculation number 87654321,
the `group.txt` file should look like this:

```text
12345678 Jane Doe
87654321 Max Mustermann
```
