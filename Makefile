# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 11:53:52 by rakman            #+#    #+#              #
#    Updated: 2025/05/19 02:47:59 by rakman           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = build/minishell
BUILD_DIR = ./build
TEST_NAME = $(BUILD_DIR)/minishell_test

# Source files within src directory
SRC_MAIN = src/main.c
SRC_DIRS = a_command \
        b_lexer \
        c_parser \
        d_executor \
        e_builtins \
        f_utils

# Libft source files
LIBFT_SRCS = $(wildcard lib/libft/*.c)

# Collect all source files
SRC_BASE = $(SRC_MAIN) \
        $(foreach dir,$(SRC_DIRS),$(wildcard src/$(dir)/*.c))

# Filter out duplicate signal handler files
SRC_FILES = $(filter-out src/f_utils/handle_signals.c src/f_utils/signal_handlers.c, $(SRC_BASE))

# Test source files
TEST_SRCS = src/f_tests/test_framework.c \
    src/f_tests/test_command.c \
    src/f_tests/test_lexer.c \
    src/f_tests/test_parser.c \
    src/f_tests/test_extra.c \
    $(wildcard src/a_command/*.c) \
    $(wildcard src/b_lexer/*.c) \
    $(wildcard src/c_parser/*.c) \
    $(wildcard src/e_utils/*.c) \
    $(LIBFT_SRCS)

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -Werror -fsanitize=address -fsanitize=undefined -fsanitize=leak -g
LDFLAGS = -lreadline

all: $(NAME)

run: all
	./$(NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(NAME): $(SRC_FILES) $(LIBFT_SRCS) | $(BUILD_DIR)
	cc $(CFLAGS) $(SRC_FILES) $(LIBFT_SRCS) $(LDFLAGS) -o $(NAME)

$(TEST_NAME): $(TEST_SRCS) | $(BUILD_DIR)
	cc $(CFLAGS) $(TEST_SRCS) $(LDFLAGS) -o $(TEST_NAME)

test: $(TEST_NAME)
	./$(TEST_NAME)

clean:
	rm -f $(NAME)
	rm -f $(TEST_NAME)

fclean: clean
	rm -rf $(BUILD_DIR)

re: fclean all

retest: fclean test

.PHONY: all run test clean fclean re retest
