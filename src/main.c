/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:30:36 by rakman            #+#    #+#             */
/*   Updated: 2025/05/19 02:02:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/__minishell.h"

void shell_loop(char *prompt, char **envp_main)
{
    char            *command;
    int             should_exit;
    t_token_list    *tokens;
	ast_node        *root_node;
    t_variable_list *shell_variables;
    int             current_exit_status;
    char            *expanded;
    char            *concated;

    should_exit = 0;
    current_exit_status = 0;
    shell_variables = init_variable_list();
    if (!shell_variables)
    {
        perror("minishell: shell_loop: failed to initialize variable list");
        return;
    }
    load_initial_env(shell_variables, envp_main);
    while (true)
    {
        command = get_command(prompt, &should_exit);
        if (command == NULL)
        {
            if (should_exit == 1)
            {
                free_variable_list(shell_variables);
                free(prompt);
                rl_clear_history();
                exit(current_exit_status);
            }
            else
                continue;
        }
        expanded = get_expanded(command, &current_exit_status, shell_variables);
        concated = get_quote_trimmed(expanded);
        tokens = create_tokens(concated);
		root_node = parse_tokens(tokens);
       	if (root_node)
            execute_ast(root_node, shell_variables, &current_exit_status);
        free_ast(root_node);
      	free_token_list(tokens);
        free(concated);
        free(expanded);
        free(command);
    }
	free_variable_list(shell_variables);
}

int main(int argc, char **argv, char **envp) // envp parametresini ekleyin
{
    char *prompt;

    (void)argc;
    (void)argv;

    clear_screen();
    setup_interactive_signals();
    prompt = prepare_prompt();
    shell_loop(prompt, envp);
    return (0);
}

