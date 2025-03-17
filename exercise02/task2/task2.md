- Explain how to read these permission strings (e.g. `rw-rw-r--`).

  r... read, w... write, x... execute
  3x user, 3x group, 3x anyone else

- How do these permission strings relate to octal permissions (e.g. `0644`)?

Saved as octal number, where the leading bit represents some special info and the other 3 the permissions

| Permission | Binary | Octal |
|------------|--------|-------|
| `rw-` | `110` | `6` |
| `r--` | `100` | `4` |
| `rwx` | `111` | `7` |
| `---` | `000` | `0` |

0644 translates to rw for user, r for group and r for world

- What is the benefit of having 'groups' and 'others'?

Groups: Set permissions for a specific group of people without transfering ownership
Others/world: For everyone else, like preventing access to sensitive files

- In your opinion, what are the ideal permissions for ...
  - ... a text file used to take private notes?
        `rw-------` Only me should be able to read an modify
  - ... a shell script?
        `rwx------` x for execute, but permissions for group and world depend on what i want the script to do. Ex. Private stuff for my homefolder? Just me. exercise.sh? x for world
  - ... a config file that should not be edited?
        `r--r--r--` Only read for everyone, but again: depending on what the config is for



Hand 'task2.txt' as your solution.