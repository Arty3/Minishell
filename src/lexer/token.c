#include "minishell.h"

static
t_token_type	get_token_type(const char *raw)
{
	if (!ft_strcmp(raw, "&&"))
		return (T_AND_IF);
	if (!ft_strcmp(raw, "||"))
		return (T_OR_IF);
	if (!ft_strcmp(raw, "<<"))
		return (T_DLESS);
	if (!ft_strcmp(raw, ">>"))
		return (T_DGREAT);
	if (!ft_strcmp(raw, "<"))
		return (T_LESS);
	if (!ft_strcmp(raw, ">"))
		return (T_GREAT);
	if (!ft_strcmp(raw, "|"))
		return (T_OR);
	if (!ft_strcmp(raw, "\n"))
		return (T_NEWLINE);
	if (!ft_strcmp(raw, "("))
		return (T_LPAREN);
	if (!ft_strcmp(raw, ")"))
		return (T_RPAREN);
	return (T_WORD);
}

t_token	*lexer_create_token(char *raw)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->type = get_token_type(raw);
	token->raw = raw;
	return (token);
}

static
void	lexer_apply_token_flags(t_token *token)
{
	size_t	i;

	i = 0;
	while (token->raw[i] != '\0')
	{
		if (token->raw[i] == '\"' || token->raw[i] == '\'')
			token->flags |= F_QUOTED;
		if (token->raw[i] == '$')
			token->flags |= F_HASDOLLAR;
		if (token->raw[i] == '\\' && !is_quote(token->raw[i + 1]))
			token->flags |= F_ESCAPED;
		++i;
	}
}

t_token	*lexer_append_token(t_lexer *lexer, char *raw)
{
	t_token	*token;
	t_list	*node;

	token = lexer_create_token(raw);
	if (token == NULL)
		return (NULL);
	lexer_apply_token_flags(token);
	node = ft_init_lst(token);
	if (node == NULL)
	{
		lexer_destroy_token(token);
		return (NULL);
	}
	ft_append_lst(&lexer->tokens, node);
	return (token);
}

void	lexer_destroy_token(t_token *token)
{
	if (token == NULL)
		return ;
	if (token->raw != NULL)
		free(token->raw);
	free(token);
}
