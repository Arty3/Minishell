#ifndef EXPANDER_H

# define EXPANDER_H

# include "libft.h"
# include "parser.h"

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}	t_quote_type;

typedef enum e_expander_flag
{
	QUOTE_REMOVAL = 1 << 0,
	VARIABLE_EXPANSION = 1 << 1,
	TILDE_EXPANSION = 1 << 2,
	FILENAME_EXPANSION = 1 << 3,
	/* apply heredoc escaping rules */
	HEREDOC = 1 << 4,
	/* keep all words in a single token */
	NO_SPLITTING = 1 << 5,
}	t_expander_flag;

typedef struct s_shell	t_shell;

typedef struct s_expander
{
	size_t			pos;
	t_quote_type	state;
	size_t			quote_pos;
	char			*buffer;
	t_list			*tokens; /* t_token */
	t_list			*wildcards; /* t_wildcard_pos */
	t_expander_flag	flags;
	t_shell			*shell;
}	t_expander;

typedef struct s_wildcard_pos
{
	size_t	index;
}	t_wildcard_pos;

void			expander_init(t_expander *expander, t_expander_flag flags);
void			expander_destroy(t_expander *expander);
bool			expander_expand_command(t_command *command, t_shell *shell);
bool			expander_expand_filename(t_expander *expander,
					t_token *expanded_token);
char			*expander_expand_redir_name(t_redir *redir,
					const char **__vars);
char			*expander_expand_heredoc(t_redir *redir, t_shell *shell);

bool			expander_expand_single_quote(t_expander *expander,
					t_token *token);
bool			expander_expand_double_quote(t_expander *expander,
					t_token *token, const char **__vars);
bool			expander_expand_no_quote(t_expander *expander, t_token *token,
					const char **__vars);
bool			expander_expand_variable(t_expander *expander, t_token *token,
					const char **__vars);
t_list			*expander_expand_token(t_expander *expander, t_token *token,
					const char **__vars);
bool			expander_expand_variable_args(t_expander *expander,
					const char *value);

t_wildcard_pos	*wildcard_pos_create(size_t index);
bool			expander_add_wildcard(t_expander *expander, const char *value);

bool			expander_append_token(t_expander *expander, t_token *token);
bool			expander_push_buffer(t_expander *expander);

t_quote_type	get_quote_type(int32_t c);
bool			should_escape(t_expander *expander, t_token *token);

char			*buffer_append_char(char *buffer, int32_t c);
char			*buffer_append_word(char *buffer, const char *word);

#endif
