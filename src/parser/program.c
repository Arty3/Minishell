#include "minishell.h"

static
t_list	*parser_parse_list(t_parser *parser)
{
	t_token		*token;
	t_list		*and_or_list;
	char		msg[MAX_ERROR_LEN];

	token = lexer_peek(parser->lexer);
	if (token == NULL)
		return (NULL);
	parser_skip_linebreaks(parser);
	if (token->type == T_AND_IF || token->type == T_OR_IF)
		return (parser_set_error(parser, "missing expressions before"
				" `||' or `&&'"), NULL);
	and_or_list = parser_parse_and_or_list(parser);
	token = lexer_peek(parser->lexer);
	if (token != NULL)
	{
		ft_memset(msg, 0, sizeof(msg));
		ft_strlcat(msg, "parse error near token `", MAX_ERROR_LEN);
		ft_strlcat(msg, token->raw, MAX_ERROR_LEN);
		ft_strlcat(msg, "\'", MAX_ERROR_LEN);
		parser_set_error(parser, msg);
		return (parser_destroy_and_or_list(&and_or_list), NULL);
	}
	return (and_or_list);
}

t_program	*parser_parse_program(t_parser *parser)
{
	t_program	*program;

	program = ft_calloc(1, sizeof(t_program));
	if (program == NULL)
		return (NULL);
	program->node_type = T_PROGRAM;
	program->and_or_list = parser_parse_list(parser);
	return (program);
}

void	parser_destroy_program(t_program *program)
{
	if (program == NULL)
		return ;
	if (program->and_or_list != NULL)
		parser_destroy_and_or_list(&program->and_or_list);
	free(program);
}
