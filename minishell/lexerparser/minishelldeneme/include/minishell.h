/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:00:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <errno.h>

# define PROMPT "minishell$ "

extern int	g_signal;
extern int	g_last_status;

typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	struct s_cmd	*next;
} t_cmd;

typedef struct s_env
{
	char		*key;
	char		*value;
	struct s_env	*next;
} t_env;

void	minishell_loop(t_env *env);
void	handle_signal(int signo);
int		parse_and_execute(char *line, t_env **env);
void	free_cmds(t_cmd *cmd);
void	free_env(t_env *env);

int		builtin_echo(char **argv);
int		builtin_cd(char **argv, t_env **env);
int		builtin_pwd(void);
int		builtin_export(char **argv, t_env **env);
int		builtin_unset(char **argv, t_env **env);
int		builtin_env(t_env *env);
int		builtin_exit(char **argv);

char	*get_env_value(t_env *env, const char *key);
void	set_env_value(t_env **env, const char *key, const char *value);
void	unset_env_value(t_env **env, const char *key);
t_env	*init_env(char **envp);
char	**env_to_envp(t_env *env);
void	free_envp(char **envp);

int		is_builtin(const char *cmd);
int		exec_builtin(char **argv, t_env **env);
char	*find_in_path(const char *cmd, t_env *env);

#endif
