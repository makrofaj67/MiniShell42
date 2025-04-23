#ifndef CELL_H
# define CELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

char	**split_line(char *line);
char	**fill_tokens(char **tokens, char *token, int position, int bufsize);
void	free_tokens(char **tokens);
char	*find_command_in_path(const char *cmd);
char	*search_in_path(char *path_copy, const char *cmd);
char	*create_full_path(const char *dir, const char *cmd);
int		execute_external_command(char *command_path, char **args, char **env);
void	process_command(char **args, char **env, int *shell_active);
void	handle_empty_or_null(char **args, int *shell_active);
int		execute_cd_command(char **args);
int		is_builtin_command(char *command);
int		execute_builtin_command(char **args, int *shell_active);

#endif

