/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_redirections.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:40:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/22 11:40:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static int	handle_input_redirection(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "minishell: %s: %s\n", redir->filename, strerror(errno));
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_output_redirection(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		fprintf(stderr, "minishell: %s: %s\n", redir->filename, strerror(errno));
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_append_redirection(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		fprintf(stderr, "minishell: %s: %s\n", redir->filename, strerror(errno));
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirections(command_value *cmd)
{
	int	i;
	int	ret;

	if (!cmd || !cmd->redirections)
		return (0);
	cmd->stdin_backup = dup(STDIN_FILENO);
	cmd->stdout_backup = dup(STDOUT_FILENO);
	i = 0;
	while (cmd->redirections[i])
	{
		if (cmd->redirections[i]->type == REDIR_IN)
			ret = handle_input_redirection(cmd->redirections[i]);
		else if (cmd->redirections[i]->type == REDIR_OUT)
			ret = handle_output_redirection(cmd->redirections[i]);
		else if (cmd->redirections[i]->type == REDIR_APPEND)
			ret = handle_append_redirection(cmd->redirections[i]);
		else if (cmd->redirections[i]->type == REDIR_HEREDOC)
			ret = handle_heredoc(cmd->redirections[i]);
		if (ret != 0)
			return (ret);
		i++;
	}
	return (0);
}

int	restore_redirections(command_value *cmd)
{
	if (!cmd)
		return (0);
	if (cmd->stdin_backup != -1)
	{
		dup2(cmd->stdin_backup, STDIN_FILENO);
		close(cmd->stdin_backup);
		cmd->stdin_backup = -1;
	}
	if (cmd->stdout_backup != -1)
	{
		dup2(cmd->stdout_backup, STDOUT_FILENO);
		close(cmd->stdout_backup);
		cmd->stdout_backup = -1;
	}
	return (0);
}
