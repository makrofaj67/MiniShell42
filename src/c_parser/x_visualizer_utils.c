/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_visualizer_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:12:30 by rakman            #+#    #+#             */
/*   Updated: 2025/05/07 11:34:49 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/**
 * @brief Creates a formatted indentation string for tree visualization
 * 
 * @param depth Current depth level in the tree
 * @param is_last Whether this is the last child at this level
 * @param prefix The prefix string to build upon
 * @return char* Formatted indentation string
 */
char	*get_tree_branch(int depth, int is_last, char *prefix)
{
	char	*new_prefix;
	char	*branch;
	size_t	needed_size;

	if (depth == 0)
		return (strdup(""));
	if (is_last)
		branch = "└── ";
	else
		branch = "├── ";
	needed_size = strlen(prefix) + strlen(branch) + 1;
	new_prefix = malloc(needed_size);
	if (!new_prefix)
		return (strdup(""));
	snprintf(new_prefix, needed_size, "%s%s", prefix, branch);
	return (new_prefix);
}

/**
 * @brief Creates the prefix for the next level of the tree
 * 
 * @param depth Current depth level in the tree
 * @param is_last Whether this is the last child at this level
 * @param prefix The current prefix string
 * @return char* Prefix for the next level
 */
char	*get_next_prefix(int depth, int is_last, char *prefix)
{
	char	*next_prefix;
	char	*extension;
	size_t	needed_size;

	if (depth == 0)
		return (strdup(""));
	if (is_last)
		extension = "    ";
	else
		extension = "│   ";
	needed_size = strlen(prefix) + strlen(extension) + 1;
	next_prefix = malloc(needed_size);
	if (!next_prefix)
		return (strdup(""));
	snprintf(next_prefix, needed_size, "%s%s", prefix, extension);
	return (next_prefix);
}

/**
 * @brief Visualizes a redirection with ASCII art
 * 
 * @param redir The redirection to visualize
 */
void	visualize_redirection(t_redirection *redir)
{
	char	*redir_symbol;

	redir_symbol = "?";
	if (redir->type == REDIR_IN)
		redir_symbol = "<";
	else if (redir->type == REDIR_OUT)
		redir_symbol = ">";
	else if (redir->type == REDIR_APPEND)
		redir_symbol = ">>";
	else if (redir->type == REDIR_HEREDOC)
		redir_symbol = "<<";
	printf(" %s %s", redir_symbol, redir->filename);
}
