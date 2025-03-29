#include "minishell.h"

t_token	*lexer_peek(t_lexer *lexer)
{
	if (lexer->current_token == NULL)
		return (NULL);
	return ((t_token *)lexer->current_token->content);
}

t_token	*lexer_pop(t_lexer *lexer)
{
	t_token	*token;

	if (lexer->current_token == NULL)
		return (NULL);
	token = lexer->current_token->content;
	lexer->current_token = lexer->current_token->next;
	return (token);
}

t_token	*lexer_copy_token(t_token *token)
{
	t_token	*copied_token;

	copied_token = ft_calloc(1, sizeof(t_token));
	if (copied_token == NULL)
		return (NULL);
	copied_token->raw = ft_strdup(token->raw);
	copied_token->flags = token->flags;
	copied_token->type = token->type;
	return (copied_token);
}
