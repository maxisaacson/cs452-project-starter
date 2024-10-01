#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <errno.h>
#include <readline/history.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include "../src/lab.h"

#define MAX_JOBS 64

static struct job job_list[MAX_JOBS];
static int job_count = 0;

// Task 5: Get shell prompt, return default if env variable is not set
char *get_prompt(const char *env) {
    char *prompt = getenv(env);
    if (!prompt) {
        prompt = "shell>";
    }
    char *result = strdup(prompt);
    if (!result) {
        perror("strdup failed");
        exit(EXIT_FAILURE);
    }
    return result;
}

// Function to parse the command line
char **cmd_parse(char const *line) {
    char **args = malloc(ARG_MAX * sizeof(char *));
    if (!args) {
        perror("malloc failed");
        return NULL;  // Handle memory allocation failure
    }

    char *token;
    char *input_copy = strdup(line);  // Make a copy of the input line for tokenization
    if (!input_copy) {
        perror("strdup failed");
        free(args);
        return NULL;  // Handle memory allocation failure
    }

    int index = 0;
    token = strtok(input_copy, " ");  // Split on spaces

    while (token != NULL && index < ARG_MAX - 1) {
        args[index++] = strdup(token);  // Duplicate each token for storage
        if (!args[index - 1]) {  // Check if strdup failed
            perror("strdup failed");
            // Free previously allocated memory before returning
            for (int i = 0; i < index - 1; i++) {
                free(args[i]);
            }
            free(args);
            free(input_copy);
            return NULL;
        }
        token = strtok(NULL, " ");
    }
    args[index] = NULL; 

    free(input_copy); 
    return args; 
}

// Function to free the allocated memory for command arguments
void cmd_free(char **args) {
    if (args) {
        for (int i = 0; args[i] != NULL; i++) {
            free(args[i]);  // Free each duplicated string
        }
        free(args);  // Free the array itself
    }
}

bool do_builtin(struct shell *sh, char **argv) {
    if (argv[0] == NULL) {
        return false;
    }

    if (strcmp(argv[0], "exit") == 0) {
        sh_destroy(sh);
        exit(0);
    } else if (strcmp(argv[0], "cd") == 0) {
        change_dir(argv+1);
        return true;
    } else if (strcmp(argv[0], "history") == 0) {
        handle_history();
        return true;
    } else if (strcmp(argv[0], "jobs") == 0) {
        print_jobs();
        return true;
    }

    return false;
}

// Task 3: Trim whitespace from the input line
char *trim_white(char *line) {
    char *end;

    while (isspace((unsigned char) *line)) line++;

    if (*line == 0) {
        return line;
    }

    end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char) *end)) end--;

    *(end + 1) = '\0';
    return line;
}

void sh_init(struct shell *sh) {
    sh->shell_is_interactive = isatty(STDIN_FILENO);
    sh->shell_terminal = STDIN_FILENO;
    sh->prompt = NULL;
}

void sh_destroy(struct shell *sh) {
    if (sh->prompt) {
        free(sh->prompt);
    }
}

// Task 6: Change directory (cd command)
int change_dir(char **args) {
    // If args is NULL or the first argument is NULL, use HOME directory
    if (args == NULL || args[0] == NULL || strlen(args[0]) == 0) {
        char *home = getenv("HOME");
        if (!home) {
            struct passwd *pw = getpwuid(getuid());
            home = pw->pw_dir;
        }
        if (chdir(home) != 0) {
            perror("cd failed");
            return -1;
        }
    } else {
        if (chdir(args[0]) != 0) {
            perror("cd failed");
            return -1;
        }
    }

    return 0;
}

void handle_history() {
    HIST_ENTRY **the_history_list = history_list();
    if (the_history_list) {
        for (int i = 0; the_history_list[i]; i++) {
            printf("%d: %s\n", i + history_base, the_history_list[i]->line);
        }
    }
}

void add_job(pid_t pid, const char *command) {
    if (job_count < MAX_JOBS) {
        job_list[job_count].pid = pid;
        strncpy(job_list[job_count].command, command, ARG_MAX);
        job_list[job_count].job_id = job_count + 1;
        job_list[job_count].status = 0; // Running
        job_count++;
    } else {
        printf("Job list is full!\n");
    }
}

// Task 10: Update job statuses
void update_jobs() {
    for (int i = 0; i < job_count; i++) {
        int result = waitpid(job_list[i].pid, NULL, WNOHANG);
        if (result == 0) {
            job_list[i].status = 0; // Running
        } else if (result == -1) {
            job_list[i].status = 1; // Done
        }
    }
}

void print_jobs() {
    for (int i = 0; i < job_count; i++) {
        if (job_list[i].status == 0) {
            printf("[%d] %d Running %s &\n", job_list[i].job_id, job_list[i].pid, job_list[i].command);
        } else {
            printf("[%d] Done %s &\n", job_list[i].job_id, job_list[i].command);
        }
    }
}

void execute_command(char *command) {
    pid_t pid = fork();
    
    if (pid == 0) {
        handle_signals();

        char *args[ARG_MAX];
        char *token = strtok(command, " ");
        int i = 0;

        while (token != NULL && i < ARG_MAX - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (execvp(args[0], args) == -1) {
            perror("exec failed");
        }

        exit(1);
    } else if (pid > 0) {
        add_job(pid, command);
        update_jobs();
    } else {
        perror("Fork failed");
    }
}
void print_version() {
    printf("Shell version: %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
}

void handle_signals() {
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}
