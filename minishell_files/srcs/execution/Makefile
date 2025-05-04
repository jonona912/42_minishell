# execution/Makefile

NAME = execution.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I. -I../includes -Ibuilt_in_commands -I..

# Source files
EXEC_SRCS = execution.c \
            execution_utils.c \
            execute_command.c \
            execute_command_utils.c \
            here_doc.c \
            here_doc_processing.c \
            redirections.c \
            utils_ex.c \
            built_in_checks.c

BUILTIN_SRCS = built_in_commands/built_in_commands.c \
               built_in_commands/built_in_commands_2.c \
               built_in_commands/utils_built.c \
               built_in_commands/export_env.c \
               built_in_commands/built_in_commands_utils.c

ALL_SRCS = $(EXEC_SRCS) $(BUILTIN_SRCS)

# Object files directory
OBJS_DIR = objs
LIB_DIR = $(OBJS_DIR)
OBJS = $(addprefix $(OBJS_DIR)/, $(ALL_SRCS:.c=.o))
LIB = $(LIB_DIR)/$(NAME)

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $(OBJS)

# Rule for execution directory .c files
$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Rule for built_in_commands directory .c files
$(OBJS_DIR)/built_in_commands/%.o: built_in_commands/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
