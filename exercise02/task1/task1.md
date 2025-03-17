# Using Make to Automate Other Tasks  

## 1. What Does It Do?

### What Does the Makefile Do?  
This `Makefile` automates the creation of a ZIP archive for an exercise submission. It:  

- Defines `USERNAME` and `EXERCISE` to name the archive (`../exc02_csbb8841.zip`).  
- Excludes unnecessary files (`.vscode`, `.idea`, `.DS_Store`, etc.).  
- Provides three rules:  
  - **`all`** → Default rule, calls `zip`.  
  - **`zip`** → Cleans old ZIP files and creates a new one while excluding junk files.  
  - **`clean`** → Runs `make clean` in all `task?` subdirectories.  

## 2. How Would You Call It?  
### How to Use It?  
Run `make` (or `make zip`) to create the archive. Use `make clean` to remove old files.  

## 3. Where Are You Supposed to Put It?  
### Where to Put It?  
Place the `Makefile` in the root of the exercise directory. It expects subdirectories like `task1`, `task2`, etc.  

## 4. Can You Think of an Additional Use Case for Make?  
### Additional Use Cases for `make`  
- **Automating tests:**  
  ```make
  .PHONY: test
  test:
  	pytest tests/
  ```
- **Deploying a project:**  
  ```make
  .PHONY: deploy
  deploy:
  	scp -r . user@server:/path/to/deploy
  ```
- **Generating documentation:**  
  ```make
  .PHONY: docs
  docs:
  	doxygen Doxyfile
  ```
- **Building and running a C program:**  
  ```make
  .PHONY: run
  run:
  	gcc -o program main.c && ./program
  ```

`make` is useful beyond compiling code—it simplifies repetitive tasks.