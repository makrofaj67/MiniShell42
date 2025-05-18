/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:30:36 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 20:24:42 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/__minishell.h"
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>

/* Global exit status variable to track command return values */
int g_exit_status = 0;

/*
** Primary execution loop for the shell program
** Continuously prompts for and processes user commands
** Handles the read-evaluate-print loop pattern of a command-line interface
** Controls the lifetime of command, token, and execution objects
**
** @param prompt: The shell prompt string to display to the user
** @param envp: Array of environment variables for command execution
*/

void shell_loop(char *prompt, char **envp) {
  char *command;
  int should_exit;
  t_token_list *tokens;
  ast_node *root_node;
  t_env *env_list;
  t_env *export_list;
  int exit_status;

  should_exit = 0;
  env_list = NULL;
  export_list = NULL;
  create_env(&env_list);    // Çevre değişkenlerini liste olarak yükle
  create_env(&export_list); // Export listesini de oluştur

  while (true) {
    command = get_command(prompt, &should_exit);
    if (command == NULL) {
      if (should_exit == 1)
        exit(EXIT_SUCCESS);
      continue;
    }
    char *expanded = get_expanded(command, exit_status, env_list);
    // root_node = parse_tokens(tokens);

    // Display the AST structure when a command is entered
    // if (root_node)
    // {
    // 	visualize_ast(root_node);
    // 	// AST ağacını kullanarak komutu yürüt
    // 	execute_ast(root_node, &exit_status, &env_list, &export_list);
    // }
    //
  //  free_token_list(tokens);
    // free_ast(root_node);
    free(command);
  }
}

/*
** Entry point for the MiniShell42 program
** Initializes the shell environment, signal handlers, and prompt
** Starts the main command processing loop and performs cleanup on exit
**
** @param argc: Count of command-line arguments (unused but required by C
*standard)
** @param argv: Array of command-line argument strings (unused but required by C
*standard)
** @param envp: Array of environment variable strings passed from the parent
*process
** @return: Exit status code (0 for normal termination)
*/
int main(int argc, char **argv, char **envp) {
  char *prompt;

  (void)argc;
  (void)argv;
  clear_screen();
  setup_interactive_signals();
  prompt = prepare_prompt(envp);
  shell_loop(prompt, envp);
  free(prompt);
  rl_clear_history();
  return (0);
}
