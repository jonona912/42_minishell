NAME = minishell
TESTER = token_tester
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address
INCLUDE = minishell_files/includes/minishell.h minishell_files/srcs/execution/includes/execution.h

SRCS_DIR = minishell_files/srcs
TOKENIZER_DIR = tokenizer
PARSER_DIR = parser
EXECUTE_DIR = execution

SRCS = main_minishell.c \
		main_utils.c \
		print_structs.c \
		$(TOKENIZER_DIR)/handle_wildcard.c \
		$(TOKENIZER_DIR)/tokenizer_helper_1.c \
		$(TOKENIZER_DIR)/token_lst_functions.c \
		$(TOKENIZER_DIR)/tokenizer_utils.c \
		$(TOKENIZER_DIR)/tokenizer_utils_2.c \
		$(TOKENIZER_DIR)/tokenizer_utils_utils.c \
		$(TOKENIZER_DIR)/tokenizer.c \
		$(PARSER_DIR)/ast_binary_tree_functions.c \
		$(PARSER_DIR)/is_functions.c \
		$(PARSER_DIR)/parser.c \
		$(PARSER_DIR)/utils_par.c \
		$(PARSER_DIR)/parser_helper_1.c \
		$(PARSER_DIR)/parser_parse_word.c \
		$(PARSER_DIR)/arg_return.c \
		$(PARSER_DIR)/arg_return_utils.c \
		$(PARSER_DIR)/return_executable_path.c \
		$(PARSER_DIR)/redirection_functions.c \
		$(PARSER_DIR)/wildcard_functions.c \
		$(PARSER_DIR)/parse_cmd.c

# Test files
TEST_DIR = tests
TEST_SRC = $(TEST_DIR)/token_tester.c

OBJS_DIR = minishell_files/objs
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
TEST_OBJ = $(addprefix $(OBJS_DIR)/, $(TEST_SRC:.c=.o))

EXECUTE_LIB_DIR = $(SRCS_DIR)/$(EXECUTE_DIR)/objs
EXECUTE_LIB = $(EXECUTE_LIB_DIR)/execution.a

PARSER_LIB_DIR = $(SRCS_DIR)/$(PARSER_DIR)/objs
PARSER_LIB = $(PARSER_LIB_DIR)/parser.a

LIBFT_DIR = libft
LIBFT = libft.a

all: $(NAME)

$(NAME): $(OBJS) $(EXECUTE_LIB) $(PARSER_LIB) $(INCLUDE) $(LIBFT_DIR)/$(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(EXECUTE_LIB) $(PARSER_LIB) $(LIBFT_DIR)/$(LIBFT) -lreadline

$(EXECUTE_LIB): $(INCLUDE)
	$(MAKE) -C $(SRCS_DIR)/$(EXECUTE_DIR)

$(PARSER_LIB): $(INCLUDE)
	$(MAKE) -C $(SRCS_DIR)/$(PARSER_DIR)

$(TESTER): $(TEST_OBJ) $(filter-out $(OBJS_DIR)/main_minishell.o, $(OBJS)) $(EXECUTE_LIB) $(LIBFT_DIR)/$(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFT_DIR)/$(LIBFT) -lreadline

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/$(TOKENIZER_DIR)/%.o: $(SRCS_DIR)/$(TOKENIZER_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/$(PARSER_DIR)/%.o: $(SRCS_DIR)/$(PARSER_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_DIR)/$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

test: $(TESTER)
	./$(TESTER)

clean:
	rm -rf $(OBJS_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(SRCS_DIR)/$(EXECUTE_DIR) clean
	$(MAKE) -C $(SRCS_DIR)/$(PARSER_DIR) clean

fclean: clean
	rm -f $(NAME) $(TESTER)
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(SRCS_DIR)/$(EXECUTE_DIR) fclean
	$(MAKE) -C $(SRCS_DIR)/$(PARSER_DIR) fclean

re: fclean all

.PHONY: all clean fclean re test
