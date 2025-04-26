# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: luka <luka@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/26 22:00:00 by luka              #+#    #+#             #
#    Updated: 2025/04/26 22:00:00 by luka             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iinclude
SRCS = \
	src/minishell.c \
	src/builtins/builtins.c \
	src/builtins/builtins_utils.c \
	src/builtins/builtins_echo.c \
	src/builtins/builtins_env.c \
	src/env/env.c \
	src/executor/executor.c \
	src/parser/parser.c \
	src/parser/parser_utils.c \
	src/parser/redirection.c \
	src/signals/signals.c \
	src/utils/utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
