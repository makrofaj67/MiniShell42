/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:45:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/22 11:45:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static void	write_heredoc_content(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	handle_heredoc(t_redirection *redir)
{
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		fprintf(stderr, "minishell: pipe: %s\n", strerror(errno));
		return (1);
	}
	write_heredoc_content(pipe_fd[1], redir->filename);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	return (0);
}
