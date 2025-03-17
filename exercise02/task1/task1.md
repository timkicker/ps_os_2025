## What does it do?

This makefile helps us creating a perfect submission zip for our OLAT-course. 

```
USERNAME = csXXXXXX
EXERCISE = 02
```
Those variables are for us to fill in (used to generate the archive-name)

```
EXCLUDE_PATTERNS = "**.vscode/*" "**.idea/*" "**__MACOSX/*" "**.DS_Store/*" "**.dSYM/*"
```
Files and folders to exclude (IDE and OS specific)

```
ARCHIVE= "../exc$(EXERCISE)_$(USERNAME).zip"
```
Puts the zipname together and sets its location to one folder upwords the filetree.

```
.PHONY: clean
clean:
	@for dir in task?; do \
	  $(MAKE) -C "$$dir" clean; \
	done
```
Runs seperate make clean commands for each direktory

```
.PHONY: zip
zip: clean
	$(RM) $(ARCHIVE)
	zip -r $(ARCHIVE) . --exclude $(EXCLUDE_PATTERNS)
```
Removes the old archive and compresses a new one without the excluded files and folders

## How would you call it?

submission-helper-for-us-incompetent-students

## Where are you supposed to put it?

Rootfolder of your excercise

## Can you think of an additional use case for make?

You may use it for scripting tasks in general, for ex. tasks whom require multiple shell commands etc.