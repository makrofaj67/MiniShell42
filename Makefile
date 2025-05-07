# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 11:53:52 by rakman            #+#    #+#              #
#    Updated: 2025/05/07 14:23:03 by rakman           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = build/minishell
BUILD_DIR = ./build
TEST_NAME = $(BUILD_DIR)/minishell_test

# Kaynak dosyalarını wildcard ile toplama
SRC_DIRS = main.c \
		a_command/*.c \
		b_lexer/*.c \
		c_parser/*.c \
		d_executor/*.c \
		e_utils/*.c
		
# Libft kaynak dosyaları
LIBFT_SRCS = lib/libft/ft_strjoin.c

# Test kaynak dosyaları
TEST_SRCS = src/f_tests/test_framework.c \
	src/f_tests/test_command.c \
	src/f_tests/test_lexer.c \
	src/f_tests/test_parser.c \
	src/f_tests/test_extra.c \
	src/a_command/*.c \
	src/b_lexer/*.c \
	src/c_parser/*.c \
	src/e_utils/*.c \
	$(LIBFT_SRCS)

# Tüm kaynak dosya yollarını oluştur
SRCS = $(addprefix src/, $(SRC_DIRS))

# Derleme işlemi için flag'ler
CFLAGS = -fsanitize=address -fsanitize=undefined -fsanitize=leak -g
LDFLAGS = -lreadline

all: $(NAME)

run: all
	./build/minishell

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(NAME): $(SRCS) $(LIBFT_SRCS) | $(BUILD_DIR)
	cc $(CFLAGS) $(SRCS) $(LIBFT_SRCS) $(LDFLAGS) -o $(NAME)

$(TEST_NAME): | $(BUILD_DIR)
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
