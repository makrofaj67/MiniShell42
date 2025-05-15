/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_tokenize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:14:35 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

t_token_list	*tokenize_command(char *command, int exit_status)
{
	t_token_list	*token_list;

	token_list = create_token_list();
	if (token_list == NULL)
		return (NULL);
	// TODO: Implement actual tokenization logic here
	// For now, we're just returning an empty token list
	print_tokens(token_list);
	return (token_list);
}
