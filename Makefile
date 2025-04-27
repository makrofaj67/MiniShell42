# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/27 16:30:54 by rakman            #+#    #+#              #
#    Updated: 2025/04/27 17:02:30 by rakman           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
BUILD_DIR = ./build
SRCS = ./src/main.c ./src/lexer/lexer.c ./src/utils/main_utils.c

all: $(NAME)

run: all
	./build/minishell

$(BUILD_DIR):
	mkdir -p build

$(NAME): $(SRCS) | $(BUILD_DIR)
	cc -fsanitize=address -fsanitize=undefined -fsanitize=leak -g $(SRCS) -lreadline -o ./$(BUILD_DIR)/minishell
