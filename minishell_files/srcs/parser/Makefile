NAME = parser.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -Iincludes -I..

# Source files
SRCS = ast_binary_tree_functions.c \
       is_functions.c \
       parse_cmd.c \
       parser.c \
       parser_helper_1.c \
       parser_parse_word.c \
       redirection_functions.c \
       return_executable_path.c \
       utils_par.c \
       wildcard_functions.c \
       arg_return.c \
       arg_return_utils.c

# Object files directory
OBJS_DIR = objs
LIB_DIR = $(OBJS_DIR)
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
LIB = $(LIB_DIR)/$(NAME)

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $(OBJS)

# Rule for parser directory .c files
$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
