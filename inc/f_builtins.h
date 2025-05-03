/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_builtins.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 21:45:32 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 21:48:59 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef F_BUILTINS_H
# define F_BUILTINS_H

# include "__minishell.h"
# include "d_executor.h"

/**
 * @brief Check if a command is a builtin
 * 
 * @param cmd Command name to check
 * @return int 1 if builtin, 0 otherwise
 */
int is_builtin(char *cmd);

/**
 * @brief Execute a builtin command
 * 
 * @param cmd Command structure with arguments
 * @param context Execution context
 * @return int Exit status of the builtin command
 */
int execute_builtin(command_value *cmd, t_exec_context *context);

/**
 * @brief Implementation of echo builtin with -n option
 * 
 * @param args Command arguments
 * @param envp Environment variables
 * @return int Exit status
 */
int builtin_echo(char **args, char **envp);

/**
 * @brief Implementation of cd builtin
 * 
 * @param args Command arguments
 * @param envp Environment variables
 * @return int Exit status
 */
int builtin_cd(char **args, char **envp);

/**
 * @brief Implementation of pwd builtin
 * 
 * @param args Command arguments
 * @param envp Environment variables
 * @return int Exit status
 */
int builtin_pwd(char **args, char **envp);

/**
 * @brief Implementation of export builtin
 * 
 * @param args Command arguments
 * @param envp Environment variables
 * @return int Exit status
 */
int builtin_export(char **args, char **envp);

/**
 * @brief Implementation of unset builtin
 * 
 * @param args Command arguments
 * @param envp Environment variables
 * @return int Exit status
 */
int builtin_unset(char **args, char **envp);

/**
 * @brief Implementation of env builtin
 * 
 * @param args Command arguments
 * @param envp Environment variables
 * @return int Exit status
 */
int builtin_env(char **args, char **envp);

/**
 * @brief Implementation of exit builtin
 * 
 * @param args Command arguments
 * @param envp Environment variables
 * @return int Exit status
 */
int builtin_exit(char **args, char **envp);

#endif