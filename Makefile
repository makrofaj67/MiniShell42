# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 11:53:52 by rakman            #+#    #+#              #
#    Updated: 2025/04/30 23:29:19 by rakman           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = build/minishell
BUILD_DIR = ./build

# Kaynak dosyalarını wildcard ile toplama
SRC_DIRS = main.c \
		a_command/*.c \
		b_lexer/*.c \
		c_parser/*.c \
		d_executor/*.c \
		e_utils/*.c
		
# Libft kaynak dosyaları
LIBFT_SRCS = lib/libft/ft_strjoin.c

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

clean:
	rm -f $(NAME)

fclean: clean
	rm -rf $(BUILD_DIR)

re: fclean all

.PHONY: all run clean fclean re
