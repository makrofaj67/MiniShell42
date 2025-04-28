/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:42:46 by rakman            #+#    #+#             */
/*   Updated: 2025/04/28 19:35:35 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef E_UTILS_H
# define E_UTILS_H
//Main_Utils
void	clear_screen(void);
void	print_prompt(void);
void	handle_signals(void);
void	signal_handler_func(int signum);
char	*prepare_prompt(char **envp);

#endif
