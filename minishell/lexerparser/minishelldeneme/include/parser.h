/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:30:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

char	**split_args(const char *line, t_env *env, int last_status);
char	**split_pipes(const char *line);
void	free_pipes(char **cmds);
void	handle_redirection(char *cmd, int *in_fd, int *out_fd);
char	*expand_vars(const char *str, t_env *env, int last_status);

#endif
