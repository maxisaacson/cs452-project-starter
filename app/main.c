#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "../src/lab.h"

#define MAX_JOBS 64  // Maximum number of jobs

int main(int argc, char **argv) {
    UNUSED(argc);
    UNUSED(argv);
    int opt;
    struct shell sh;
    sh_init(&sh);  // Initialize the shell

    handle_signals();
    while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
            case 'v':
                print_version();  
                return 0;         // Exit after printing version
            default:
                fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
                return 1;        
        }
    }
    char *line;
    char *prompt;

    while (1) {
        prompt = get_prompt("MY_PROMPT");
        line = readline(prompt);
        free(prompt);

        if (!line) {
            break; // EOF
        }
        char *trimmed_line = trim_white(line);

        if (strlen(trimmed_line) > 0) {
            add_history(trimmed_line);
            char **parsed_args = cmd_parse(trimmed_line);
            if (!do_builtin(&sh, parsed_args)) {
                execute_command(trimmed_line);
            }
            cmd_free(parsed_args); // Free the parsed arguments
        }
        free(line);  // Only free `line` after `trimmed_line` is done
        update_jobs();  
    }

    sh_destroy(&sh); 
    return 0;
}
