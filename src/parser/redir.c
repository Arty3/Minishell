#include "minishell.h"

static
bool	is_token_direct_redir(t_token *token)
{
	return (token->type == T_LESS || token->type == T_GREAT
		|| token->type == T_DLESS || token->type == T_DGREAT);
}

static
bool	parse_redir(t_parser *parser, t_redir *redir)
{
	t_token	*token;

	token = lexer_peek(parser->lexer);
	if (token == NULL || !is_token_direct_redir(token))
		return (false);
	redir->type = token->type;
	lexer_pop(parser->lexer);
	token = lexer_peek(parser->lexer);
	if (token == NULL || token->type != T_WORD)
	{
		parser_set_error(parser, "expected a name after redirection");
		return (false);
	}
	lexer_pop(parser->lexer);
	redir->name = lexer_copy_token(token);
	if (redir->name == NULL)
	{
		parser_set_error(parser, "failed to allocate memory");
		return (false);
	}
	return (true);
}

t_redir	*parser_parse_redir(t_parser *parser)
{
	t_token	*token;
	t_redir	*redir;

	token = lexer_peek(parser->lexer);
	if (token == NULL)
		return (NULL);
	redir = ft_calloc(1, sizeof(t_redir));
	if (redir == NULL)
		return (NULL);
	redir->node_type = T_REDIR;
	if (!parse_redir(parser, redir))
	{
		parser_destroy_redir(redir);
		return (NULL);
	}
	return (redir);
}

void	parser_destroy_redir(t_redir *redir)
{
	if (redir == NULL)
		return ;
	if (redir->raw != NULL)
		free(redir->raw);
	if (redir->name != NULL)
		lexer_destroy_token(redir->name);
	if (redir->value != NULL)
		free(redir->value);
	free(redir);
}
