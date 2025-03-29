# **************************************************************************** #

NAME		= minishell

CC			= cc

DEPS 		?= ./Libft/bin/Libft.a
LIBS		?= readline
INCL 		+= ./include ./Libft/include

CFLAGS  	= -Wall -Wextra -Werror -Wstrict-prototypes -Wmissing-prototypes	\
			  -std=gnu17 -fstack-protector-strong -Wno-pointer-arith -O2

CFLAGS		+= $(addprefix -I,$(INCL))

SRC			= src/signal/signal.c						\
			  src/prompt/prompt.c						\
			  src/env/env.c								\
			  src/env/env_display.c						\
			  src/env/shlvl.c							\
			  src/env/env_append.c						\
			  src/main.c								\
			  src/builtins/builtin_cd.c					\
			  src/builtins/builtin_echo.c				\
			  src/builtins/builtin_env.c				\
			  src/builtins/builtin_export.c				\
			  src/builtins/builtin_pwd.c				\
			  src/builtins/builtin_unset.c				\
			  src/builtins/builtin_exit.c				\
			  src/builtins/execute.c					\
			  src/builtins/utils.c						\
			  src/cmd/cmd.c								\
			  src/cmd/error.c							\
			  src/cmd/redir.c							\
			  src/execute/and_or.c						\
			  src/execute/command.c						\
			  src/execute/execute.c						\
			  src/execute/pipeline.c					\
			  src/execute/redir.c						\
			  src/execute/heredoc.c						\
			  src/execute/executable_command.c			\
			  src/execute/resolve_binary.c				\
			  src/expand/append.c						\
			  src/expand/command.c						\
			  src/expand/escape.c						\
			  src/expand/expand.c						\
			  src/expand/filename.c						\
			  src/expand/quote.c						\
			  src/expand/redir.c						\
			  src/expand/variable.c						\
			  src/expand/words.c						\
			  src/lexer/lexer.c							\
			  src/lexer/peek.c							\
			  src/lexer/token.c							\
			  src/lexer/word.c							\
			  src/parser/and_or.c						\
			  src/parser/command.c						\
			  src/parser/linebreak.c					\
			  src/parser/parser.c						\
			  src/parser/pipeline.c						\
			  src/parser/program.c						\
			  src/parser/redir.c						\
			  src/parser/simple_command.c				\
			  src/utils/base_path.c						\
			  src/utils/path_join.c						\
			  src/utils/print_error.c					\
			  src/utils/sglob.c							\
			  src/utils/shglob.c						\
			  src/utils/smatch.c						\
			  src/utils/fd_bitmap.c						\
			  src/utils/is_valid_identifier.c			\
			  src/utils/is_valid_number.c				\
			  src/utils/sorted_array.c

OBJ 		= $(SRC:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

Libft: Libft/bin/Libft.a

Libft/bin/Libft.a:
	@$(MAKE) -C ./Libft

$(NAME): $(OBJ) $(DEPS)
	@$(CC) $(CFLAGS) $^ $(addprefix -l,$(LIBS)) -o $@

clean:
	@$(MAKE) clean -C ./Libft
	@$(RM) -f ${OBJ}

fclean: clean
	@$(MAKE) fclean -C ./Libft
	@$(RM) -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re Libft
