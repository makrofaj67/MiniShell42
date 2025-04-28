# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 11:53:52 by rakman            #+#    #+#              #
#    Updated: 2025/04/28 22:04:52 by rakman           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
BUILD_DIR = ./build
SRCS = ./src/main.c ./src/b_lexer/lexer.c ./src/e_utils/main_utils.c ./src/a_command/commander.c ./src/a_command/linked_list_helpers.c \
	   ./src/a_command/quote_backslash_helpers.c ./src/a_command/commander_helpers.c

all: $(NAME)

run: all
	./build/minishell

$(BUILD_DIR):
	mkdir -p build

$(NAME): $(SRCS) | $(BUILD_DIR)
	cc -fsanitize=address -fsanitize=undefined -fsanitize=leak -g $(SRCS) -lreadline -o ./$(BUILD_DIR)/minishell
