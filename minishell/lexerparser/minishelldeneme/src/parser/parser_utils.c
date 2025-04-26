/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:35:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <glob.h>
#include <ctype.h>

char	*expand_vars(const char *str, t_env *env, int last_status)
{
	char	result[4096];
	int		ri;
	int		i;

	ri = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				char buf[16];
				int j;

				sprintf(buf, "%d", last_status);
				j = 0;
				while (buf[j])
					result[ri++] = buf[j++];
				i += 2;
			}
			else if (isalpha(str[i + 1]) || str[i + 1] == '_')
			{
				int k;
				char var[128];
				int vi;
				char *val;
				int j;

				k = i + 1;
				vi = 0;
				while (isalnum(str[k]) || str[k] == '_')
					var[vi++] = str[k++];
				var[vi] = 0;
				val = get_env_value(env, var);
				if (val)
				{
					j = 0;
					while (val[j])
						result[ri++] = val[j++];
				}
				i = k;
			}
			else
				result[ri++] = str[i++];
		}
		else
			result[ri++] = str[i++];
	}
	result[ri] = 0;
	return (strdup(result));
}

char	**split_pipes(const char *line)
{
	char	**cmds;
	char	*copy;
	char	*saveptr;
	char	*token;
	int		i;

	cmds = malloc(sizeof(char *) * 32);
	copy = strdup(line);
	saveptr = NULL;
	token = strtok_r(copy, "|", &saveptr);
	i = 0;
	while (token && i < 31)
	{
		while (*token == ' ' || *token == '\t')
			token++;
		cmds[i] = strdup(token);
		i++;
		token = strtok_r(NULL, "|", &saveptr);
	}
	cmds[i] = NULL;
	free(copy);
	return (cmds);
}

void	free_pipes(char **cmds)
{
	int i;
	i = 0;
	while (cmds && cmds[i])
	{
		free(cmds[i]);
		i++;
	}
	free(cmds);
}

static int	split_args_helper(const char *line, int *i, char **argv, t_env *env, int last_status, int ai)
{
	char	arg[1024];
	int		aj;
	int		k;
	char	*exp;
	int		ti;

	aj = 0;
	if (line[*i] == '\'')
	{
		(*i)++;
		while (line[*i] && line[*i] != '\'')
			arg[aj++] = line[(*i)++];
		(*i)++;
		arg[aj] = 0;
		argv[ai++] = strdup(arg);
	}
	else if (line[*i] == '"')
	{
		(*i)++;
		while (line[*i] && line[*i] != '"')
		{
			if (line[*i] == '$')
			{
				char tmp[256];
				ti = 0;
				tmp[ti++] = line[(*i)++];
				while (isalnum(line[*i]) || line[*i] == '_' || line[*i] == '?')
					tmp[ti++] = line[*i++];
				tmp[ti] = 0;
				exp = expand_vars(tmp, env, last_status);
				k = 0;
				while (exp[k])
					arg[aj++] = exp[k++];
				free(exp);
			}
			else
				arg[aj++] = line[(*i)++];
		}
		(*i)++;
		arg[aj] = 0;
		argv[ai++] = strdup(arg);
	}
	else
	{
		while (line[*i] && !isspace(line[*i]) && line[*i] != '<' && line[*i] != '>' && line[*i] != '|')
		{
			if (line[*i] == '$')
			{
				char tmp[256];
				ti = 0;
				tmp[ti++] = line[(*i)++];
				while (isalnum(line[*i]) || line[*i] == '_' || line[*i] == '?')
					tmp[ti++] = line[*i++];
				tmp[ti] = 0;
				exp = expand_vars(tmp, env, last_status);
				k = 0;
				while (exp[k])
					arg[aj++] = exp[k++];
				free(exp);
			}
			else
				arg[aj++] = line[(*i)++];
		}
		arg[aj] = 0;
		if (strchr(arg, '*'))
		{
			glob_t g;
			int gi;

			if (glob(arg, 0, NULL, &g) == 0)
			{
				gi = 0;
				while (gi < (int)g.gl_pathc)
					argv[ai++] = strdup(g.gl_pathv[gi++]);
				globfree(&g);
			}
			else
				argv[ai++] = strdup(arg);
		}
		else if (aj > 0)
			argv[ai++] = strdup(arg);
	}
	return (ai);
}

char	**split_args(const char *line, t_env *env, int last_status)
{
	char	**argv;
	int		i;
	int		ai;

	argv = malloc(sizeof(char *) * 128);
	i = 0;
	ai = 0;
	while (line[i])
	{
		while (isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		ai = split_args_helper(line, &i, argv, env, last_status, ai);
	}
	argv[ai] = NULL;
	return (argv);
}
