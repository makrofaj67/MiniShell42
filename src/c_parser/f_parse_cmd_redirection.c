/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_parse_cmd_redirection.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:15:30 by rakman            #+#             */
/*   Updated: 2025/05/07 17:08:42 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static void	set_redir_type(t_redirection *redir, t_token_type type)
{
	if (type == RDRT_IN)
		redir->type = REDIR_IN;
	else if (type == RDRT_OUT)
		redir->type = REDIR_OUT;
	else if (type == APPEND)
		redir->type = REDIR_APPEND;
	else if (type == HEREDOC)
		redir->type = REDIR_HEREDOC;
}

static int	setup_redirection(t_redirection *redir, t_token_node *node)
{
	set_redir_type(redir, node->type);
	redir->filename = strdup(node->next->value);
	if (redir->filename == NULL)
		return (0);
	return (1);
}

void	add_redir(t_token_node *node, t_cmdval_list *list)
{
	t_redirection	*new_redir;
	int				success;

	new_redir = (t_redirection *)malloc(sizeof(t_redirection));
	if (new_redir == NULL)
	{
		perror("malloc failed for t_redirection");
		return ;
	}
	if (!setup_redirection(new_redir, node))
	{
		perror("strdup failed for filename");
		free(new_redir);
		return ;
	}
	success = add_node_to_cmdval_list(list, new_redir);
	if (!success)
	{
		perror("add_node_to_cmdval_list failed");
		free(new_redir->filename);
		free(new_redir);
	}
}
