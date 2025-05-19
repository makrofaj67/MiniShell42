/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_utils.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:42:46 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 21:09:15 by rakman           ###   ########.fr       */
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

/* Signal handling functions */
void	setup_interactive_signals(void);
void	reset_signal_flag(void);

/* From libft */
char	*ft_strjoin(char const *s1, char const *s2);

/* Error handling functions */
void	*perror_rnull(char *message);
void	*perror_exit(char *message);

#endif
