/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_parse_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:25:48 by rakman            #+#    #+#             */
/*   Updated: 2025/05/06 16:31:02 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

command_value *parse_simple_command(t_token_list *tokens)
{
	command_value			*cmd_details;
	t_cmdval_list			*redir_list;
	t_cmdval_list			*word_list;
	t_cmdval_node			*current_word;
	t_cmdval_node			*current_redir;
	t_token_node			*current_token;
	int						i;
	int						j;

	cmd_details = (command_value *)malloc(sizeof(command_value));
	if (cmd_details == NULL)
		return (NULL);
	redir_list = init_cmdval_list();
	word_list = init_cmdval_list();
	if (!redir_list || !word_list) 
	{
		free(cmd_details);
		free_cmdval_list_with_contents(redir_list, 1);
		free_cmdval_list_with_contents(word_list, 0);
		return (NULL);
	}
	current_token = tokens->head;
	while(current_token != NULL)
	{
		if (current_token->type == RDRT_IN || current_token->type == RDRT_OUT || current_token->type == HEREDOC || current_token->type == APPEND)
		{
			if (current_token->next == NULL || current_token->next->type != WORD)
			{
				printf("Syntax Error");
				free(cmd_details);
				free_cmdval_list_with_contents(redir_list, 1);
				free_cmdval_list_with_contents(word_list, 0);
				return (NULL);
			}
			else
			{
				add_redir(current_token, redir_list);
				if (current_token->next != NULL)
					current_token = current_token->next->next;
			}
		}
		else if (current_token->type == WORD)
		{
			add_word(current_token, word_list);
			current_token = current_token->next;
		}
		else
		{
			printf("Unknown error");
			free(cmd_details);
			free_cmdval_list_with_contents(redir_list, 1);
			free_cmdval_list_with_contents(word_list, 0);
			return (NULL);
		}
	}

//redirlist
//wordlist

	//params (t_cmdval_list *redirlist, t_cmdval_list *wordlist, command_value *cmd_details)
	//return command_value
	//
	//command_value *join_cmd_details(t_cmdval_list *redirlist, t_cmdval_list *wordlist, command_value *cmd_details)

	cmd_details->arg_array = (char **)malloc(sizeof(char *) * (word_list->size + 1));
	if (cmd_details->arg_array == NULL) {
		free(cmd_details);
		free_cmdval_list_with_contents(redir_list, 1);
		free_cmdval_list_with_contents(word_list, 0);
		return (NULL);
	}
	
	i = 0;
	current_word = word_list->head;
	while(current_word != NULL)
	{
		cmd_details->arg_array[i++] = current_word->value;
		current_word = current_word->next;
	}
	cmd_details->arg_array[i] = NULL;

	cmd_details->redirections = (t_redirection **)malloc(sizeof(t_redirection *) * (redir_list->size + 1));
	if (cmd_details->redirections == NULL) {
		free(cmd_details->arg_array);
		free(cmd_details);
		free_cmdval_list_with_contents(redir_list, 1);
		free_cmdval_list_with_contents(word_list, 0);
		return (NULL);
	}
	
	j = 0;
	current_redir = redir_list->head;
	while(current_redir != NULL)
	{	
		cmd_details->redirections[j++] = (t_redirection *)current_redir->value;
		current_redir = current_redir->next;
	}
	cmd_details->redirections[j] = NULL; 
	free_cmdval_list_nodes_only(redir_list);
	free_cmdval_list_nodes_only(word_list);
	return (cmd_details);
}
