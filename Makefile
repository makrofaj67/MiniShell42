# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 11:53:52 by rakman            #+#    #+#              #
#    Updated: 2025/04/29 13:56:40 by rakman           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = build/minishell
BUILD_DIR = ./build
SRCS = ./src/main.c ./src/e_utils/main_utils.c \
	   ./src/a_command/a_commander.c ./src/a_command/b_commander_helpers.c \
	   ./src/a_command/c_get_next_lines_if_needed.c ./src/a_command/d_get_next_lines_helpers.c \
	   ./src/a_command/e_linked_list_helpers.c \
	   ./src/b_lexer/a_lexer.c ./src/b_lexer/b_lexer_helper.c \
	   ./src/b_lexer/c_print_and_get_value.c ./src/b_lexer/d_linked_list.c \
	   ./src/c_parser/parser.c ./src/d_executor/executor.c

all: $(NAME)

run: all
	./build/minishell

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(NAME): $(SRCS) | $(BUILD_DIR)
	cc -fsanitize=address -fsanitize=undefined -fsanitize=leak -g $(SRCS) -lreadline -o $(NAME)
