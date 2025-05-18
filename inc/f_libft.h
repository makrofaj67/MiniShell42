/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_libft.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 23:27:41 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 23:32:04 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef F_LIBFT_H
# define F_LIBFT_H

# include <stdlib.h>
# include <string.h>


char    *ft_strcpy(char *dst, const char *src); // Or your Libft's specific signature
int     ft_strcmp(const char *s1, const char *s2);
char    *ft_itoa(int n);
char    *ft_strdup(const char *s1);
size_t  ft_strlen(const char *s);
char    *ft_substr(char const *s, unsigned int start, size_t len);
char    *ft_strjoin(char const *s1, char const *s2);
/* String functions */

#endif

