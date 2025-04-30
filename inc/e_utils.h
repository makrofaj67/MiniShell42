/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_utils.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:42:46 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 20:35:56 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef E_UTILS_H
# define E_UTILS_H

/* Main Utils */
void	clear_screen(void);
void	print_prompt(void);
void	handle_signals(void);
void	signal_handler_func(int signum);
char	*prepare_prompt(char **envp);

/* Error handling functions */
void *perror_rnull(char *message);
void *perror_exit(char *message);

#endif
