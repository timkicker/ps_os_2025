# Use this file at your own risk.
# Always ensure that the required files are actually put into the archive,
# and that no additional files are archived.
# You can use the zipinfo command to check the file structure of an existing archive.

# --- ADJUST BELOW ----------------

# Number of people: set to 1, 2 or 3 (how many zips to create)
PEOPLE ?= 2
USERNAME_PERSON_1 = csbaXXXX
USERNAME_PERSON_2 = csbbXXXX
USERNAME_PERSON_3 = csbcXXXX
EXERCISE = 04

# Data for group.txt. Only relevant if PEOPLE is not 1.
PERSON_1 = Max Mustermann
PERSON_2 = Gordon Freeman
PERSON_3 = Alyx Vance
MAT_NUM_1 = 12345678
MAT_NUM_2 = 87654321
MAT_NUM_3 = 98765432

# ---------------------------------

# This excludes some common directories automatically.
# Also ignores all binaries and README.md.
EXCLUDE_PATTERNS = "**.vscode/*" "**.idea/*" "**__MACOSX/*" "**.DS_Store/*" "**.dSYM/*" "**/*.o" "**/a.out" "README.md"

ARCHIVE_PERSON_1 = "./exc$(EXERCISE)_$(USERNAME_PERSON_1).zip"
ARCHIVE_PERSON_2 = "./exc$(EXERCISE)_$(USERNAME_PERSON_2).zip"
ARCHIVE_PERSON_3 = "./exc$(EXERCISE)_$(USERNAME_PERSON_3).zip"

# --- TARGETS ---

.PHONY: all
all: prepare zip

# prepare: execute clean, group, format, and setperms in order.
.PHONY: prepare
prepare: clean group format setperms

.PHONY: clean
clean:
	@echo "Cleaning task folders in exercise$(EXERCISE)..."
	@for dir in ./exercise$(EXERCISE)/task_*; do \
		if [ -d "$$dir" ]; then \
			-$(MAKE) -C "$$dir" clean || echo "Warning: no clean target in $$dir"; \
		fi; \
	done
	@rm -f exercise$(EXERCISE)/group.txt

.PHONY: group
group:
ifeq ($(strip $(PEOPLE)),1)
	@echo "Single submission detected: group.txt will not be created."
else
	@echo "Creating group.txt in exercise$(EXERCISE)..."
	@echo "$(MAT_NUM_1) $(PERSON_1)" > exercise$(EXERCISE)/group.txt
ifneq ($(filter 2 3,$(strip $(PEOPLE))),)
	@echo "$(MAT_NUM_2) $(PERSON_2)" >> exercise$(EXERCISE)/group.txt
endif
ifneq ($(filter 3,$(strip $(PEOPLE))),)
	@echo "$(MAT_NUM_3) $(PERSON_3)" >> exercise$(EXERCISE)/group.txt
endif
endif

.PHONY: format
format:
	@echo "Formatting all .c files in exercise$(EXERCISE)..."
	@find exercise$(EXERCISE) -type f -name "*.c" -exec clang-format -i {} \;

.PHONY: setperms
setperms:
	@echo "Setting world-read permissions for all files in exercise$(EXERCISE)..."
	@find exercise$(EXERCISE) -type f -exec chmod a+r {} \;

.PHONY: zip
zip: prepare
	@mkdir -p submission
	$(RM) ./submission/$(ARCHIVE_PERSON_1)
ifeq ($(strip $(PEOPLE)),1)
	(cd exercise$(EXERCISE) && zip -r ../submission/$(ARCHIVE_PERSON_1) . --exclude $(EXCLUDE_PATTERNS))
else ifeq ($(strip $(PEOPLE)),2)
	$(RM) ./submission/$(ARCHIVE_PERSON_2)
	(cd exercise$(EXERCISE) && zip -r ../submission/$(ARCHIVE_PERSON_1) . --exclude $(EXCLUDE_PATTERNS))
	(cd exercise$(EXERCISE) && zip -r ../submission/$(ARCHIVE_PERSON_2) . --exclude $(EXCLUDE_PATTERNS))
else ifeq ($(strip $(PEOPLE)),3)
	$(RM) ./submission/$(ARCHIVE_PERSON_2)
	$(RM) ./submission/$(ARCHIVE_PERSON_3)
	(cd exercise$(EXERCISE) && zip -r ../submission/$(ARCHIVE_PERSON_1) . --exclude $(EXCLUDE_PATTERNS))
	(cd exercise$(EXERCISE) && zip -r ../submission/$(ARCHIVE_PERSON_2) . --exclude $(EXCLUDE_PATTERNS))
	(cd exercise$(EXERCISE) && zip -r ../submission/$(ARCHIVE_PERSON_3) . --exclude $(EXCLUDE_PATTERNS))
endif
