# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 11:53:52 by rakman            #+#    #+#              #
#    Updated: 2025/04/29 15:25:00 by rakman           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = build/minishell
BUILD_DIR = ./build
SRCS = ./src/main.c ./src/b_lexer/lexer.c ./src/e_utils/main_utils.c ./src/a_command/commander.c ./src/a_command/linked_list_helpers.c \
	   ./src/a_command/quote_backslash_helpers.c ./src/a_command/commander_helpers.c ./src/b_lexer/type_and_value_getters.c \
	   ./src/b_lexer/linked_list_helpers.c ./src/b_lexer/print_token_list.c ./src/b_lexer/token_len_utils.c ./src/b_lexer/token_char_helpers.c \
	   ./src/a_command/commander_utils.c ./src/a_command/command_continuity.c

all: $(NAME)

run: all
	./build/minishell

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(NAME): $(SRCS) | $(BUILD_DIR)
	cc -fsanitize=address -fsanitize=undefined -fsanitize=leak -g $(SRCS) -lreadline -o $(NAME)
