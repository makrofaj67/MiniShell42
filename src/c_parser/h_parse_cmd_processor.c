/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_parse_cmd_processor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:36:42 by rakman            #+#    #+#             */
/*   Updated: 2025/05/07 17:12:15 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/**
 * @brief Handle a redirection token during parsing
 * 
 * @param token Pointer to current token pointer (will be updated)
 * @param redir_list List to add redirection to
 * @return int 1 on success, 0 on failure
 */
static int	handle_redirection_token(t_token_node **token,
				t_cmdval_list *redir_list)
{
	if ((*token)->next == NULL || (*token)->next->type != WORD)
	{
		printf("Syntax Error: Missing argument for redirection\n");
		return (0);
	}
	add_redir(*token, redir_list);
	if ((*token)->next != NULL)
		*token = (*token)->next->next;
	else
		*token = NULL;
	return (1);
}

/**
 * @brief Handle a word token during parsing
 * 
 * @param token Pointer to current token pointer (will be updated)
 * @param word_list List to add word to
 * @return int 1 on success, 0 on failure
 */
static int	handle_word_token(t_token_node **token, t_cmdval_list *word_list)
{
	add_word(*token, word_list);
	*token = (*token)->next;
	return (1);
}

/**
 * @brief Process a token during command parsing
 * 
 * @param token Pointer to current token pointer (will be updated)
 * @param r_list Redirection list
 * @param w_list Word list
 * @return int 1 on success, 0 on failure
 */
static int	process_token_node(t_token_node **token, t_cmdval_list *r_list,
				t_cmdval_list *w_list)
{
	if ((*token)->type == RDRT_IN || (*token)->type == RDRT_OUT
		|| (*token)->type == HEREDOC || (*token)->type == APPEND)
	{
		return (handle_redirection_token(token, r_list));
	}
	else if ((*token)->type == WORD)
	{
		return (handle_word_token(token, w_list));
	}
	else
	{
		printf("Unknown token type encountered\n");
		return (0);
	}
}

/**
 * @brief Fill command value lists with tokens
 * 
 * @param tokens Token list to process
 * @param r_list Redirection list to fill
 * @param w_list Word list to fill
 * @return t_cmdval_list** Array of both lists or NULL on failure
 */
t_cmdval_list	**fill_cmdval_lists(t_token_list *tokens, t_cmdval_list *r_list,
				t_cmdval_list *w_list)
{
	t_cmdval_list	**double_list;
	t_token_node	*current_token;

	current_token = tokens->head;
	double_list = (t_cmdval_list **)malloc(sizeof(t_cmdval_list *) * 2);
	if (double_list == NULL)
		return (NULL);
	while (current_token != NULL)
	{
		if (!process_token_node(&current_token, r_list, w_list))
		{
			free_cmdval_list_with_contents(r_list, 1);
			free_cmdval_list_with_contents(w_list, 0);
			free(double_list);
			return (NULL);
		}
	}
	double_list[0] = r_list;
	double_list[1] = w_list;
	return (double_list);
}