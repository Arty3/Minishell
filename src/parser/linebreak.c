#include "minishell.h"

void	parser_skip_linebreaks(t_parser *parser)
{
	t_token	*token;

	token = lexer_peek(parser->lexer);
	while (token != NULL && token->type == T_NEWLINE)
	{
		lexer_pop(parser->lexer);
		token = lexer_peek(parser->lexer);
	}
}
