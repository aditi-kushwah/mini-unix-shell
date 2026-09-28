# Mini Unix Shell

A lightweight Unix-like command-line shell built from scratch in C using POSIX system calls and Linux process-management concepts.

## 📌 Project Overview

Mini Unix Shell is a command-line interpreter developed in C to understand how Unix shells work internally.

The project implements process creation, command execution, input/output redirection, pipelines, background processes, environment variable expansion, command history, signal handling, and zombie-process prevention.

## 🚀 Features

* Execute external Linux commands
* Built-in `cd` command
* Built-in `pwd` command
* Built-in `echo` command
* Built-in `help` command
* Built-in `history` command
* Input redirection using `<`
* Output redirection using `>`
* Append redirection using `>>`
* Single and multiple command pipelines using `|`
* Background process execution using `&`
* Environment variable expansion such as `$HOME` and `$USER`
* Double-quoted string support
* `Ctrl+C` signal handling
* `SIGCHLD` handling for automatic zombie-process cleanup
* Error handling for invalid commands and files
* Command history with a maximum of 50 commands

## 🛠️ Technologies Used

* **Language:** C
* **Operating System:** Linux / WSL
* **Compiler:** GCC
* **Build Tool:** Make
* **Version Control:** Git / GitHub
* **System Calls / APIs:** `fork()`, `execvp()`, `wait()`, `waitpid()`, `pipe()`, `dup2()`, `open()`, `signal()`, `getenv()`

## 📂 Project Structure

```text
mini-unix-shell/
│
├── main.c
├── Makefile
├── README.md
├── .gitignore
│
├── include/
│   ├── builtins.h
│   └── parser.h
│
└── src/
    ├── builtins.c
    └── parser.c
```

## ⚙️ Build and Run

Clone the repository and enter the project directory:

```bash
git clone https://github.com/aditi-kushwah/mini-unix-shell.git
cd mini-unix-shell
```

Build the project:

```bash
make
```

Run the shell:

```bash
./mini-shell
```

To remove the compiled executable:

```bash
make clean
```

## 💻 Example Usage

### Basic Commands

```text
mini-shell> pwd
/home/hp/mini-unix-shell

mini-shell> echo Hello
Hello

mini-shell> history
1 pwd
2 echo Hello
3 history
```

### Environment Variables

```text
mini-shell> echo $USER
hp

mini-shell> echo $HOME
/home/hp

mini-shell> echo My home is $HOME
My home is /home/hp
```

### Quoted Strings

```text
mini-shell> echo "Hello World"
Hello World
```

### Input Redirection

```text
mini-shell> cat < input.txt
```

### Output Redirection

```text
mini-shell> echo Hello > output.txt
```

### Append Redirection

```text
mini-shell> echo First > test.txt
mini-shell> echo Second >> test.txt
mini-shell> cat test.txt
First
Second
```

### Pipes

```text
mini-shell> ls | grep main
main.c
```

Multiple pipes are also supported:

```text
mini-shell> cat test.txt | grep Second | wc -l
1
```

Pipes can also be combined with environment variables:

```text
mini-shell> echo $HOME | grep /home
/home/hp
```

### Background Processes

```text
mini-shell> sleep 10 &
[Background process started: 12805]
mini-shell>
```

The shell remains available while the process runs in the background.

Completed background processes are automatically reaped using `SIGCHLD` handling to prevent zombie processes.

### Signal Handling

Foreground processes can be interrupted using:

```text
Ctrl+C
```

The child process receives the default `SIGINT` behavior while the shell remains active.

## 🧠 Concepts Demonstrated

This project demonstrates practical understanding of:

* Process creation using `fork()`
* Program execution using `execvp()`
* Parent-child process management
* Process synchronization using `wait()` / `waitpid()`
* Inter-process communication using `pipe()`
* File descriptor manipulation using `dup2()`
* File handling using `open()`
* Input/output redirection
* Background process management
* Unix signals
* Zombie-process prevention
* Environment variables
* Command parsing
* Quoted string handling
* Error handling
* Modular C programming
* Makefile-based compilation
* Git and GitHub workflow

## 🔮 Future Improvements

Possible future improvements include:

* Command history navigation using arrow keys
* Additional shell built-ins
* Improved command parsing and quoting
* More robust syntax validation
* Advanced job-control features
* Support for more shell operators

## 👩‍💻 Author

**Aditi Kushwah**

GitHub: https://github.com/aditi-kushwah
