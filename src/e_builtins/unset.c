/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:38 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 12:00:38 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

// The unset_cmd function now takes char **args to handle multiple arguments
// and t_variable_list *variables for the new environment system.
void	unset_cmd(char **args, t_variable_list *variables)
{
	int	i;

	if (!args || !variables)
		return ;
	// Iterate through each argument passed to unset
	// args[0] is "unset", so we start from args[1]
	i = 1;
	while (args[i])
	{
		// TODO: Add validation for variable names (e.g., cannot unset "PATH" if it's special,
		// or check for invalid characters). For now, we directly unset.
		unset_variable(variables, args[i]);
		i++;
	}
	// Unset builtin typically doesn't produce an error for non-existent variables
	// and has an exit status of 0 unless an option is invalid (not supported here)
	// or a variable name is invalid (validation not yet implemented).
}
