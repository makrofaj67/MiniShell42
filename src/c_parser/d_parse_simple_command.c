/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_parse_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:25:48 by rakman            #+#    #+#             */
/*   Updated: 2025/05/07 11:06:12 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static int	init_args_array(command_value *cmd, t_cmdval_list *word_list)
{
	t_cmdval_node	*current;
	int				i;

	cmd->arg_array = (char **)malloc(sizeof(char *) * (word_list->size + 1));
	if (cmd->arg_array == NULL)
		return (0);
	i = 0;
	current = word_list->head;
	while (current != NULL)
	{
		cmd->arg_array[i++] = current->value;
		current = current->next;
	}
	cmd->arg_array[i] = NULL;
	return (1);
}

static int	init_redir_array(command_value *cmd, t_cmdval_list *redir_list)
{
	t_cmdval_node	*current;
	int				j;

	cmd->redirections = (t_redirection **)malloc(sizeof(t_redirection *)
			* (redir_list->size + 1));
	if (cmd->redirections == NULL)
		return (0);
	j = 0;
	current = redir_list->head;
	while (current != NULL)
	{
		cmd->redirections[j++] = (t_redirection *)current->value;
		current = current->next;
	}
	cmd->redirections[j] = NULL;
	return (1);
}

static int	cmd_alloc_error(command_value *cmd, t_cmdval_list *r_list,
						t_cmdval_list *w_list)
{
	if (cmd)
		free(cmd);
	free_cmdval_list_with_contents(r_list, 1);
	free_cmdval_list_with_contents(w_list, 0);
	return (0);
}

command_value	*join_cmd_details(t_cmdval_list *redir_list,
						t_cmdval_list *word_list)
{
	command_value	*cmd;

	cmd = (command_value *)malloc(sizeof(command_value));
	if (cmd == NULL)
		return (NULL);
	if (!init_args_array(cmd, word_list))
	{
		cmd_alloc_error(cmd, redir_list, word_list);
		return (NULL);
	}
	if (!init_redir_array(cmd, redir_list))
	{
		free(cmd->arg_array);
		cmd_alloc_error(cmd, redir_list, word_list);
		return (NULL);
	}
	free_cmdval_list_nodes_only(redir_list);
	free_cmdval_list_nodes_only(word_list);
	return (cmd);
}

command_value	*parse_simple_command(t_token_list *tokens)
{
	command_value	*cmd_details;
	t_cmdval_list	*redir_list;
	t_cmdval_list	*word_list;
	t_cmdval_list	**val_lists;

	redir_list = init_cmdval_list();
	word_list = init_cmdval_list();
	if (!redir_list || !word_list)
	{
		free_cmdval_list_with_contents(redir_list, 1);
		free_cmdval_list_with_contents(word_list, 0);
		return (NULL);
	}
	val_lists = fill_cmdval_lists(tokens, redir_list, word_list);
	if (!val_lists)
		return (NULL);
	free(val_lists);
	cmd_details = join_cmd_details(redir_list, word_list);
	return (cmd_details);
}
