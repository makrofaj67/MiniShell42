/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:40:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int	redir_heredoc(char *lim, int *in_fd)
{
	char	tmpfile[64];
	int		fd;
	char	*line;
	size_t	len;

	snprintf(tmpfile, sizeof(tmpfile), "/tmp/minishell_heredocXXXXXX");
	fd = mkstemp(tmpfile);
	if (fd == -1)
		return (1);
	line = NULL;
	len = 0;
	while (1)
	{
		printf("> ");
		if (getline(&line, &len, stdin) == -1)
			break ;
		if (!strcmp(line, lim) || !strncmp(line, lim, strlen(lim)))
			break ;
		write(fd, line, strlen(line));
	}
	free(line);
	lseek(fd, 0, SEEK_SET);
	*in_fd = fd;
	return (0);
}

void	handle_redirection(char *cmd, int *in_fd, int *out_fd)
{
	char	*redir;
	char	*end;
	char	tmp;

	redir = cmd;
	while (*redir)
	{
		if (*redir == '<')
		{
			if (*(redir + 1) == '<')
			{
				*redir = 0;
				redir += 2;
				while (*redir == ' ' || *redir == '\t')
					redir++;
				end = redir;
				while (*end && *end != ' ' && *end != '\t' && *end != '>' && *end != '<')
					end++;
				tmp = *end;
				*end = 0;
				redir_heredoc(redir, in_fd);
				*end = tmp;
			}
			else
			{
				*redir = 0;
				redir++;
				while (*redir == ' ' || *redir == '\t')
					redir++;
				end = redir;
				while (*end && *end != ' ' && *end != '\t' && *end != '>' && *end != '<')
					end++;
				tmp = *end;
				*end = 0;
				*in_fd = open(redir, O_RDONLY);
				*end = tmp;
			}
		}
		else if (*redir == '>')
		{
			if (*(redir + 1) == '>')
			{
				*redir = 0;
				redir += 2;
				while (*redir == ' ' || *redir == '\t')
					redir++;
				end = redir;
				while (*end && *end != ' ' && *end != '\t' && *end != '>' && *end != '<')
					end++;
				tmp = *end;
				*end = 0;
				*out_fd = open(redir, O_WRONLY | O_CREAT | O_APPEND, 0644);
				*end = tmp;
			}
			else
			{
				*redir = 0;
				redir++;
				while (*redir == ' ' || *redir == '\t')
					redir++;
				end = redir;
				while (*end && *end != ' ' && *end != '\t' && *end != '>' && *end != '<')
					end++;
				tmp = *end;
				*end = 0;
				*out_fd = open(redir, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				*end = tmp;
			}
		}
		else
			redir++;
	}
}
