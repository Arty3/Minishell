#ifndef LEXER_H

# define LEXER_H

# include "libft.h"

typedef enum e_token_type
{
	// &
	T_AND,
	// &&
	T_AND_IF,
	// |
	T_OR,
	// ||
	T_OR_IF,
	// <<
	T_DLESS,
	// <
	T_LESS,
	// >>
	T_DGREAT,
	// >
	T_GREAT,
	// (
	T_LPAREN,
	// )
	T_RPAREN,
	// \n
	T_NEWLINE,
	T_WORD,
}	t_token_type;

typedef enum e_token_flag
{
	F_QUOTED = 1 << 0, // `'` `"'
	F_HASDOLLAR = 1 << 1, // `$'
	F_ESCAPED = 1 << 2 // `\'
}	t_token_flag;

typedef struct s_token
{
	t_token_type	type;
	char			*raw;
	t_token_flag	flags;
}	t_token;

typedef struct s_lexer
{
	char	*buf;
	char	*error;
	size_t	pos;
	t_list	*tokens; /* t_token */
	t_list	*current_token; /* t_token */
}	t_lexer;

t_lexer	*lexer_create(const char *buf);
void	lexer_set_error(t_lexer *lexer, const char *message);
void	lexer_destroy(t_lexer *lexer);
bool	lexer_tokenize(t_lexer *lexer);

ssize_t	get_word_length(t_lexer *lexer);

t_token	*lexer_peek(t_lexer *lexer);
t_token	*lexer_pop(t_lexer *lexer);

t_token	*lexer_create_token(char *raw);
t_token	*lexer_append_token(t_lexer *lexer, char *raw);
void	lexer_destroy_token(t_token *token);
t_token	*lexer_copy_token(t_token *token);

#endif
