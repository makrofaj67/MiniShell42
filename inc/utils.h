/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:42:46 by rakman            #+#    #+#             */
/*   Updated: 2025/04/27 18:43:17 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

//Main_Utils
void	clear_screen(void);
void	print_prompt(void);
void	handle_signals(void);
void	signal_handler_func(int signum);
char	*prepare_prompt(char **envp);

#endif
