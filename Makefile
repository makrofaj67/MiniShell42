# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 11:53:52 by rakman            #+#    #+#              #
#    Updated: 2025/05/22 15:30:00 by rakman           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = build/minishell
BUILD_DIR = ./build

SRC_MAIN = src/main.c

SRC_ENV = src/0_env_management/a_variable_list_ops.c \
          src/0_env_management/b_load_shell_variables.c \
          src/0_env_management/c_findget_varible_node.c \
          src/0_env_management/d_set_variable.c \
          src/0_env_management/e_unset_variable.c \
          src/0_env_management/f_free_list.c

SRC_CMD = src/a_command/a_get_command.c \
          src/a_command/b_is_command_blank.c \
          src/a_command/c_parenthesis_status.c \
          src/a_command/d_has_unclosed_quotes.c \
          src/a_command/e_stack_ops.c

SRC_LEX = src/b_lexer/a_get_expanded.c \
          src/b_lexer/b_is_expanding_needed.c \
          src/b_lexer/c_get_quote_state_at_position.c \
          src/b_lexer/c_handle_variable_expansion.c \
          src/b_lexer/d_get_quotes_trimmed_and_join.c \
          src/b_lexer/g_tokenizer.c \
          src/b_lexer/i_token_list_operations.c \
          src/b_lexer/temp_string_utils.c \
          src/b_lexer/x_token_print.c

SRC_PAR = src/c_parser/a_parse_tokens.c \
          src/c_parser/b_find_last_pipe.c \
          src/c_parser/c_parse_simple_pipe.c \
          src/c_parser/d_parse_simple_command.c \
          src/c_parser/e_parse_cmd_list.c \
          src/c_parser/f_parse_cmd_redirection.c \
          src/c_parser/g_create_ast_nodes.c \
          src/c_parser/h_parse_cmd_processor.c \
          src/c_parser/x_visualizer_ast.c \
          src/c_parser/x_visualizer_utils.c \
          src/c_parser/y_freeing_list.c \
          src/c_parser/z_freeing_ast.c

SRC_EXE = src/d_executor/a_execute_ast.c \
          src/d_executor/b_execute_external.c \
          src/d_executor/c_execute_pipe.c \
          src/d_executor/d_external_command.c \
          src/d_executor/e_redirections.c \
          src/d_executor/f_heredoc.c

SRC_BLT = src/e_builtins/a_echo_cd.c \
          src/e_builtins/b_pwd_export.c \
          src/e_builtins/c_unset_env.c \
          src/e_builtins/d_exit.c

SRC_UTL = src/f_utils/main_utils.c \
          src/f_utils/minishell_signals.c

SRC_LFT = lib/libft/ft_itoa.c \
          lib/libft/ft_split.c \
          lib/libft/ft_strchr.c \
          lib/libft/ft_strcmp.c \
          lib/libft/ft_strcpy.c \
          lib/libft/ft_strdup.c \
          lib/libft/ft_strjoin.c \
          lib/libft/ft_strlen.c \
          lib/libft/ft_substr.c

SRC_FILES = $(SRC_MAIN) $(SRC_ENV) $(SRC_CMD) $(SRC_LEX) $(SRC_PAR) $(SRC_EXE) $(SRC_BLT) $(SRC_UTL) $(SRC_LFT)

CFLAGS = -Wall -Wextra -Werror -fsanitize=address -fsanitize=undefined -fsanitize=leak -g
LDFLAGS = -lreadline

OBJ_FILES = $(SRC_FILES:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR) $(NAME)

run: all
	./$(NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/src
	mkdir -p $(BUILD_DIR)/src/0_env_management
	mkdir -p $(BUILD_DIR)/src/a_command
	mkdir -p $(BUILD_DIR)/src/b_lexer
	mkdir -p $(BUILD_DIR)/src/c_parser
	mkdir -p $(BUILD_DIR)/src/d_executor
	mkdir -p $(BUILD_DIR)/src/e_builtins
	mkdir -p $(BUILD_DIR)/src/f_utils
	mkdir -p $(BUILD_DIR)/lib/libft

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	cc $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ_FILES)
	mkdir -p $(BUILD_DIR)
	cc $(CFLAGS) $(OBJ_FILES) $(LDFLAGS) -o $(NAME)

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -rf $(BUILD_DIR)

re: fclean all

.PHONY: all run clean fclean re
