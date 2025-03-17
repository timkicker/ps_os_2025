# Changing File Permissions in Bash

To change file permissions in Linux, you can use the `chmod` command. Here's an explanation of how it works:

## Command to Change Permissions
```bash
chmod 755 filename
```

### Explanation of `755`:
- `755` breaks down into three parts: **owner**, **group**, and **others**.
- Each digit represents a combination of permissions:
  - **0**: No permission (`---`)
  - **1**: Execute (`--x`)
  - **2**: Write (`-w-`)
  - **4**: Read (`r--`)

#### Breakdown:
- **7 (Owner)**: Read (`4`) + Write (`2`) + Execute (`1`) = `rwx`
- **5 (Group)**: Read (`4`) + Execute (`1`) = `r-x`
- **5 (Others)**: Read (`4`) + Execute (`1`) = `r-x`

---

## Questions in `task2.txt`
Answer the following questions:

1. **Which command did you use to set the permissions for `task2.txt`?**
   ```bash
   chmod 400 file.txt
   ```

2. **Which command did you use to set the permissions for `file.txt`?**
   ```bash
   chmod 600 file.txt
   ```

---

## Solved Output

```bash
[csbb8841@zid-gpl s02]$ ./exercise.sh 
Congratulations, you already solved the first part of this task about permissions.
Next, follow the prompts all the way to the end.
'task2.txt' found.
Incorrect permissions for 'task2.txt'.
Required permissions: Only the user can read and write to the file.
Please set the correct permissions.
[csbb8841@zid-gpl s02]$ chmod 600 task2.txt
[csbb8841@zid-gpl s02]$ ./exercise.sh 
Congratulations, you already solved the first part of this task about permissions.
Next, follow the prompts all the way to the end.
'task2.txt' found.
'task2.txt' has correct permissions.
Incorrect permission for 'file.txt'.
Make sure 'file.txt' has only a single read permission for the user.
[csbb8841@zid-gpl s02]$ chmod 400 file.txt 
[csbb8841@zid-gpl s02]$ ./exercise.sh 
Congratulations, you already solved the first part of this task about permissions.
Next, follow the prompts all the way to the end.
'task2.txt' found.
'task2.txt' has correct permissions.
'file.txt' has correct permissions.
```

---

### Key Takeaways:
- Use `chmod` to change file permissions.
- Permissions are represented numerically:
  - `4` = Read
  - `2` = Write
  - `1` = Execute
- Combine these numbers to set permissions for the **owner**, **group**, and **others**.