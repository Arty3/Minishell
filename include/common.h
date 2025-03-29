#ifndef COMMON_H

# define COMMON_H

# include "libft.h"
# include "lexer.h"

# define PROGRAM_NAME			"minishell"

// Secondary prompt displayed when a command needs more input
// (e.g. a multi-line command)
# define PS2					"> "

// Max username size
# define USR_MAX				64

# define DEFAULT_BITMAP_SIZE	32

// Exit status
# define EXIT_QUIT				2
# define EXIT_INVALID_USAGE		2
# define EXIT_PARSER_ERROR		2 /* 258 */
# define EXIT_BIN_CANT_EXEC		126
# define EXIT_BIN_NOT_FOUND		127
# define EXIT_SIGINT			130

// Bitmap
# define CLOSE_RESET			0
# define CLOSE_PRE_EXEC_CHILD	1
# define CLOSE_IN_CHILD			2
# define CLOSE_POST_EXEC_PARENT	3
# define CLOSE_ALL 				-1

// Signals
# define SIG_TRAP_RESET 		0
# define SIG_TRAP_SHELL 		1
# define SIG_TRAP_PROMPT 		2
# define SIG_TRAP_HEREDOC 		3

// Environment
# define ENV_PRINT_NONE 		0
# define ENV_PRINT_EXPORT 		1

# define MAX_SHLVL 				1000

# define COLOR_RESET			"\033[0m"
# define COLOR_RED				"\033[1;31m"
# define COLOR_GREEN			"\033[1;32m"
# define COLOR_YELLOW			"\033[1;33m"
# define COLOR_BLUE				"\033[1;34m"
# define COLOR_MAGENTA			"\033[1;35m"
# define COLOR_CYAN				"\033[1;36m"
# define COLOR_WHITE			"\033[1;37m"

# define STPARENT				01 /* shell is within main process */
# define STSUBSHELL				02 /* shell is within subshell */

extern int	g_sigstat;

typedef struct s_glob
{
	char			*base_path;
	char			*segment;
	char			*next_segment;
	bool			has_next_segment;
	size_t			segment_index;
	size_t			segment_length;
	struct dirent	*dirent;
}	t_glob;

typedef struct s_fd_bitmap
{
	size_t	size;
	char	*bitmap;
}	t_fd_bitmap;

typedef struct s_cmd
{
	const char	*bin;
	const char	**argv;
	char		**envp;
	size_t		argc;
	bool		is_builtin;
	pid_t		pid;
}	t_cmd;

typedef enum e_builtin
{
	CD,
	PWD,
	ENV,
	ECHO,
	EXIT,
	UNSET,
	EXPORT,
	NA_BUILTIN
}	t_builtin_type;

typedef struct s_parser
{
	t_lexer	*lexer;
	char	*error;
}	t_parser;

typedef enum e_node_type
{
	T_PROGRAM,
	T_AND_OR,
	T_PIPELINE,
	T_SIMPLE_COMMAND,
	T_SUBSHELL,
	T_REDIR,
}	t_node_type;

typedef struct s_node
{
	t_node_type	node_type;
}	t_node;

typedef struct s_program
{
	t_node_type	node_type; /* T_PROGRAM */
	t_list		*and_or_list;
}	t_program;

typedef struct s_command
{
	t_node_type	node_type; /* T_SUBSHELL | T_SIMPLE_COMMAND */
}	t_command;

typedef struct s_subshell
{
	t_node_type	node_type; /* T_SUBSHELL */
	t_list		*and_or_list; /* t_and_or */
	t_list		*redirs; /* t_redir */
}	t_subshell;

typedef struct s_simple_command
{
	t_node_type	node_type; /* T_SIMPLE_COMMAND */
	t_token		*name;
	t_list		*args; /* t_token */
	t_list		*redirs; /* t_redir */
}	t_simple_command;

typedef struct s_redir
{
	t_node_type		node_type; /* T_REDIR */
	char			*raw;
	t_token			*name;
	/* this is only used for T_DLESS */
	char			*value;
	/* inferred from token */
	t_token_type	type; /* T_DLESS | T_LESS | T_DGREAT | T_GREAT */
}	t_redir;

typedef struct s_pipeline
{
	t_node_type	node_type; /* T_PIPELINE */
	t_list		*commands; /* t_command */
}	t_pipeline;

typedef struct s_and_or
{
	t_node_type		node_type; /* T_AND_OR */
	t_pipeline		*pipeline;
	t_token_type	type; /* T_AND_IF | T_OR_IF | T_OTHER */
}	t_and_or;

typedef struct s_shell
{
	char		**envp;
	char		*default_home;
	int			state;
	int			exit_status;
	t_lexer		*lexer;
	t_parser	*parser;
	t_program	*program;
	char		*current_line;
	t_fd_bitmap	*fd;
}	t_shell;

#endif
