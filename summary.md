        Max Isaacson
        CS 452
# Summary of Custom Shell Implementation
Please note that these do not correlate directly to the numbered tasks given in the handout. This is to explain how different tasks were handled and grouped together (at least thats how I designed it).
## 1. Shell Initialization
- Upon starting, the shell sets up the necessary environment, including initializing signal handling mechanisms to manage user interrupts and background jobs.
- It allocates data structures to store information about running jobs, which allows the shell to keep track of multiple processes and their states.

## 2. Command Prompt Display
- The shell displays a prompt (`shell>`) to indicate readiness for user input. This is done in a loop to allow continuous interaction until the shell is terminated.
- The prompt is designed to be user-friendly and clearly indicates the shell's state, improving the overall user experience.

## 3. Command Parsing
- User input is read as a single line and parsed into individual tokens using the `strtok` function. This process splits the input based on whitespace, allowing the shell to identify commands and their arguments.
- The parsed tokens are stored in an array, where the first token represents the command to be executed, and subsequent tokens represent arguments.
- Error handling is implemented to ensure that commands are processed correctly, and invalid input is managed gracefully.

## 4. Built-in Commands
- The shell includes several built-in commands for essential functionality (not all shown):
  - **`cd`**: Changes the current working directory. If no argument is provided, it defaults to the user's home directory. If the specified directory does not exist, an error message is displayed.
  - **`jobs`**: Displays a list of all background jobs managed by the shell. It shows their process IDs and the commands associated with each job, helping users keep track of running processes.

## 5. Process Execution
- When the user enters a command, the shell forks a new child process using `fork()`. This creates a separate process to execute the command without blocking the shell.
- The child process uses `execvp` to replace its memory space with the new command, effectively running the external program.
- If the command includes `&`, indicating it should run in the background, the parent process continues to accept new commands while the child runs in the background, allowing for concurrent operations.

## 6. Job Management
- The shell maintains an array of job structures, each containing information about the process ID and the command executed. This data structure allows for efficient tracking and management of background jobs.
- Functions are implemented to add, update, and display jobs, providing users with insights into the status of their background processes.

## 7. Signal Handling
- The shell is designed to handle user signals effectively:
  - For example: it ignores the `SIGINT` signal (triggered by `Ctrl+C`) to ensure that the shell does not terminate when this signal is received.
  - Instead, the signal is forwarded to child processes, allowing them to terminate appropriately if interrupted by the user.
- This mechanism helps maintain the shell's interactivity while ensuring that running commands can be interrupted as needed.

## 8. Shell Cleanup
- When the user decides to exit the shell (usually through `Ctrl+Z` or a dedicated exit command), the shell performs cleanup operations.
- It iterates through the job list, terminating any remaining background processes to prevent orphaned processes from lingering in the system.
- After cleaning up resources, the shell exits, providing a smooth termination experience for the user.


## Reflection
This project was the hardest programming assignment I have had to complete in at least a year. Numerous problems kept coming up as I struggled to understand what was going on with my code. The most difficult part for me was the background tasks and signal handling. I had bad coding practices as I continually would break something I had done previously with 'temporary fixes'. I'm happy with my final code as all major commands work. I found some bugs with how I handled the HOME directory, but the cd command still works correctly. The only thing I am still unsure about is the 'sleep' command, I saw it used in task 10 example but it was not mentioned elsewhere.
