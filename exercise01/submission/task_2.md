
# Man pages

## man intro

Just a small introduction to linux and how to use man in general. Also some common commands

## man man-pages

Just how to use man and common commands sorted by functions. Pretty neat for starters, that's actually how i started off a few years ago

## man file-hirarchy

Explains how the linux (also mainly unix) is structured. Looks like that's only the case if you're using systemd, wich most linux-distros use.

## man printf vs man 3 printf

First one is the shell-command but man can also be used for exploring the c-standard. That's the case for the second command wich explains the printf() function out of stdio.h



# Creating a file

## Nav and create file

Navigating is done using cd. Using it without parameters results in me jumping into the home folder. 

Via "touch" it's possible to create a file

## Output and save
Using "ls" lists all the files and directories. You may even combine it with some arguments like ls -al in order to view more details like the creation-date.

Using ">" we can write the output of a command into a file. Like "ls > ex2/i-am-a-file", wich i used earlier in the home directories

## Remote and history

I needet to look this one up but its done using the scp command. Note that this has to be executed on your local machine and not on the server

For ex: "scp ~/Downloads/myfile.txt username@zid-gpl.uibk.ac.at:~/myfolder/"

the first argument is the LOCAL path on my machine, the second one is the server itself (of course you'll have to use your credentials) and the last behind ":" (which is not a separate argument) is the filepath of the specific file you want to copy.

using "history" lists all the commands in this session.

For me it would currently be:



    1  ls
    2  pwd
    3  ls
    4  pwd
    5  ls
    6  module avail
    7  git
    8  git clone https://github.com/uibk-dps-teaching/ps_os_2025/tree/main
    9  git clone https://github.com/uibk-dps-teaching/ps_os_2025.git
    10  pwd
    11  ls
    12  rm -rf S2/
    13  pwd
    14  mkdir S2
    15  cd S2/
    16  mkdir OS
    17  cd OS
    18  git clone https://github.com/timkicker/ps_os_2025
    19  ls
    20  ls | grep *.md
    21  cd ps_os_2025/
    22  ls
    23  cd ps_os_2025/
    24  ls | grep *.md
    25  cat README.md 
    26  man grep
    27  help cd
    28  man intro
    29  cd
    30  mkdir ex2
    31  cd ex2
    32  touch "i-am-a-new-file"
    33  ls >> i-am-a-new-file 
    34  ls i-am-a-new-file 
    35  rm -rf i-am-a-new-file 
    36  touch "i-am-a-new-file"
    37  cd 
    38  ls >> ex2/i-am-a-new-file 
    39  ls ex2/i-am-a-new-file 
    40  cd
    41  ls
    42  cd ex2/
    43  rm -rf i-am-a-new-file 
    44  touch i-am-a-new-file
    45  cd ..
    46  ls
    47  cd
    48  ls -la ~ > ex2/i-am-a-new-file 
    49  cd ex2/
    50  rm i-am-a-new-file 
    51  cd ..
    52  ls
    53  cd ex2/
    54  touch i-am-a-new-file
    55  cd ..
    56  cd
    57  ls > ex2/i-am-a-new-file 
    58  cat ex2/i-am-a-new-file 
    59  history




``
