/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:00:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;
int	g_last_status = 0;

static int	exec_logical_chain(const char *line, t_env **env);
static void	exec_group(const char *p, t_env **env, const char **next);

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
	minishell_loop(env);
	free_env(env);
	return (0);
}

void	minishell_loop(t_env *env)
{
	char	*line;

	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*line)
			add_history(line);
		if (strstr(line, "&&") || strstr(line, "||"))
			exec_logical_chain(line, &env);
		else
			parse_and_execute(line, &env);
		free(line);
	}
}

static void	exec_group(const char *p, t_env **env, const char **next)
{
	int		depth;
	const char	*q;
	char		*group;
	size_t	len;

	depth = 1;
	q = p + 1;
	while (*q && depth)
	{
		if (*q == '(')
			depth++;
		else if (*q == ')')
			depth--;
		q++;
	}
	len = (size_t)(q - p - 1);
	group = strndup(p + 1, len);
	exec_logical_chain(group, env);
	free(group);
	*next = q;
}

static int	exec_logical_chain(const char *line, t_env **env)
{
	const char	*p;
	const char	*next;
	char		*cmd;
	size_t		len;
	int			is_and;

	p = line;
	while (*p)
	{
		if (*p == '(')
		{
			exec_group(p, env, &next);
			p = next;
			if (*p == '&' && *(p + 1) == '&')
				p += 2;
			else if (*p == '|' && *(p + 1) == '|')
				p += 2;
			continue ;
		}
		next = strstr(p, "&&");
		is_and = 1;
		if (!next || (strstr(p, "||") && strstr(p, "||") < next))
		{
			next = strstr(p, "||");
			is_and = 0;
		}
		len = next ? (size_t)(next - p) : strlen(p);
		cmd = strndup(p, len);
		parse_and_execute(cmd, env);
		free(cmd);
		if ((is_and && g_last_status != 0) || (!is_and && g_last_status == 0))
			break ;
		if (!next)
			break ;
		p = next + 2;
	}
	return (g_last_status);
}
