# Use this file at your own risk.
# Always ensure that the required files are actually put into the archive,
# and that no additional files are archived.
# You can use the zipinfo command to check the file structure of an existing archive.

# --- ADJUST BELOW ----------------

PEOPLE ?= 2	# amount of people (how many zips to create)
USERNAME_PERSON_1 = csbc1132
USERNAME_PERSON_2 = csbb8841
EXERCISE = 02

# Data for group.txt
# Not relevant for PEOPLE < 2
PERSON_1 = Tim Jochen Kicker
PERSON_2 = Maurits van Staalduinen
MAT_NUM_1 = 12417095
MAT_NUM_2 = 12417095

# ---------------------------------

# This excludes some common directories automatically.
# Also ignores all binaries and README.md.
EXCLUDE_PATTERNS = "**.vscode/*" "**.idea/*" "**__MACOSX/*" "**.DS_Store/*" "**.dSYM/*" "**/*.o" "**/a.out" "README.md"

ARCHIVE_PERSON_1 = "./exc$(EXERCISE)_$(USERNAME_PERSON_1).zip"
ARCHIVE_PERSON_2 = "./exc$(EXERCISE)_$(USERNAME_PERSON_2).zip"

# --- TARGETS ---

.PHONY: all
all: prepare zip

# prepare: führe clean, group, format und setperms in Reihenfolge aus.
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
	@echo "Creating group.txt in exercise$(EXERCISE)..."
	@echo "$(MAT_NUM_1) $(PERSON_1)" > exercise$(EXERCISE)/group.txt
	@echo "$(MAT_NUM_2) $(PERSON_2)" >> exercise$(EXERCISE)/group.txt

.PHONY: format
format:
	@echo "Formatting all .c files in exercise$(EXERCISE)..."
	@find exercise$(EXERCISE) -type f -name "*.c" -exec clang-format -i {} \;

.PHONY: setperms
setperms:
	@echo "Setting world-read permissions for all files in exercise$(EXERCISE)..."
	@find exercise$(EXERCISE) -type f -exec chmod a+r {} \;

.PHONY: zip
zip:
	@mkdir -p submission
	$(RM) ./submission/$(ARCHIVE_PERSON_1)
ifeq ($(strip $(PEOPLE)),1)
	(cd exercise$(EXERCISE) && zip -r ../submission/$(ARCHIVE_PERSON_1) . --exclude $(EXCLUDE_PATTERNS))
else
	$(RM) ./submission/$(ARCHIVE_PERSON_2)
	(cd exercise$(EXERCISE) && zip -r ../submission/$(ARCHIVE_PERSON_1) . --exclude $(EXCLUDE_PATTERNS))
	(cd exercise$(EXERCISE) && zip -r ../submission/$(ARCHIVE_PERSON_2) . --exclude $(EXCLUDE_PATTERNS))
endif
