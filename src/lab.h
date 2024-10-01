#ifndef LAB_H
#define LAB_H

#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <limits.h>

#define ARG_MAX 64
#define MAX_JOBS 64
#define lab_VERSION_MAJOR 1
#define lab_VERSION_MINOR 0
#define UNUSED(x) (void)x;

#ifdef __cplusplus
extern "C"
{
#endif

struct job {
    int job_id;      // Add this line
    pid_t pid;
    char command[ARG_MAX];
    bool finished;
    int status;
};



  struct shell {
    int shell_is_interactive;
    pid_t shell_pgid;
    struct termios shell_tmodes;
    int shell_terminal;
    char *prompt;
  };

  char *get_prompt(const char *env);
  int change_dir(char **dir);
  char **cmd_parse(char const *line);
  void cmd_free(char **line);
  char *trim_white(char *line);
  bool do_builtin(struct shell *sh, char **argv);
  void sh_init(struct shell *sh);
  void sh_destroy(struct shell *sh);
  void parse_args(int argc, char **argv);
  void handle_signals();
  void handle_history();
  void execute_command(char *command);
  
  // Job control functions
  void add_job(pid_t pid, const char *command);
  void update_jobs();
  void print_jobs();
  void print_version();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LAB_H
