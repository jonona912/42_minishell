NAME = minishell
TESTER = token_tester
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDE = minishell_files/includes/minishell.h

SRCS_DIR = minishell_files/srcs
TOKENIZER_DIR = tokenizer
PARSER_DIR = parser
EXECUTE_DIR = execution
COMMANDS_DIR = built_in_commands

SRCS = main_minishell.c \
		$(TOKENIZER_DIR)/tokenizer_helper_1.c \
		$(TOKENIZER_DIR)/token_lst_functions.c \
		$(TOKENIZER_DIR)/tokenizer.c \
		$(PARSER_DIR)/ast_binary_tree_functions.c \
		$(PARSER_DIR)/parser.c \
		$(PARSER_DIR)/parser_helper_1.c \
		$(PARSER_DIR)/parser_parse_word.c \
		$(PARSER_DIR)/return_executable_path.c \
		$(PARSER_DIR)/redirection_functions.c \
		$(PARSER_DIR)/wildcard_functions.c \
		$(EXECUTE_DIR)/execution.c \
		$(EXECUTE_DIR)/execution_utils.c \
		$(COMMANDS_DIR)/built_in_commands.c \
		$(COMMANDS_DIR)/built_in_commands_utils.c

# Test files
TEST_DIR = tests
TEST_SRC = $(TEST_DIR)/token_tester.c

OBJS_DIR = minishell_files/objs
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
TEST_OBJ = $(addprefix $(OBJS_DIR)/, $(TEST_SRC:.c=.o))

# simplify libft
LIBFT_DIR = libft
LIBFT = libft.a

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDE) $(LIBFT_DIR)/$(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT_DIR)/$(LIBFT) -lreadline

$(TESTER): $(TEST_OBJ) $(filter-out $(OBJS_DIR)/main_minishell.o, $(OBJS)) $(LIBFT_DIR)/$(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFT_DIR)/$(LIBFT) -lreadline

# Rule for main source file
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for tokenizer files
$(OBJS_DIR)/$(TOKENIZER_DIR)/%.o: $(SRCS_DIR)/$(TOKENIZER_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for parser files
$(OBJS_DIR)/$(PARSER_DIR)/%.o: $(SRCS_DIR)/$(PARSER_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for execution files
$(OBJS_DIR)/$(EXECUTE_DIR)/%.o: $(SRCS_DIR)/$(EXECUTE_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for built-in commands files
$(OBJS_DIR)/$(COMMANDS_DIR)/%.o: $(SRCS_DIR)/$(COMMANDS_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for test files
$(OBJS_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/$(COMMANDS_DIR)/%.o: $(COMMANDS_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_DIR)/$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

test: $(TESTER)
	./$(TESTER)

clean:
	rm -rf $(OBJS_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(TESTER)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re test
