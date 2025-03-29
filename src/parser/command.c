#include "minishell.h"

static
bool	parser_parse_subshell_suffix(t_parser *parser, t_subshell *subshell)
{
	t_redir	*redir;
	t_list	*redir_list;

	redir = parser_parse_redir(parser);
	if (redir != NULL)
	{
		redir_list = ft_init_lst(redir);
		if (redir_list == NULL)
		{
			parser_set_error(parser, "failed to allocate memory");
			return (false);
		}
		ft_append_lst(&subshell->redirs, redir_list);
		return (true);
	}
	return (false);
}

// subshell         : '(' compound_list ')'
// compound_list    :              term
//                  | newline_list term
//                  |              term separator
//                  | newline_list term separator
//                  ;
// term             : term separator and_or
//                  |                and_or
t_subshell	*parser_parse_subshell(t_parser *parser)
{
	t_token		*token;
	t_subshell	*subshell;
	t_list		*and_or_list;

	token = lexer_peek(parser->lexer);
	if (token == NULL || token->type != T_LPAREN)
		return (NULL);
	lexer_pop(parser->lexer);
	subshell = ft_calloc(1, sizeof(t_subshell));
	if (subshell == NULL)
		return (NULL);
	subshell->node_type = T_SUBSHELL;
	and_or_list = parser_parse_and_or_list(parser);
	if (and_or_list == NULL)
		return (parser_set_error(parser, "parse error near token `('"),
			parser_destroy_subshell(subshell), NULL);
	while (parser_parse_subshell_suffix(parser, subshell))
		continue ;
	token = lexer_peek(parser->lexer);
	if (token == NULL || token->type != T_RPAREN)
		return (parser_set_error(parser, "missing closing parenthesis"
				" (wanted `)')"), parser_destroy_subshell(subshell),
			parser_destroy_and_or_list(&and_or_list), NULL);
	subshell->and_or_list = and_or_list;
	return (lexer_pop(parser->lexer), subshell);
}

void	parser_destroy_subshell(t_subshell *subshell)
{
	if (subshell == NULL)
		return ;
	if (subshell->and_or_list != NULL)
		ft_clear_lst(&subshell->and_or_list,
			(void (*)(void *)) parser_destroy_and_or);
	if (subshell->redirs != NULL)
		ft_clear_lst(&subshell->redirs,
			(void (*)(void *)) parser_destroy_redir);
	free(subshell);
}

t_command	*parser_parse_command(t_parser *parser)
{
	t_subshell			*subshell_cmd;
	t_simple_command	*simple_cmd;

	subshell_cmd = parser_parse_subshell(parser);
	if (subshell_cmd != NULL || parser->error != NULL)
	{
		if (parser->error == NULL)
		{
			while (parser_parse_subshell_suffix(parser, subshell_cmd))
				continue ;
		}
		return ((t_command *) subshell_cmd);
	}
	simple_cmd = parser_parse_simple_command(parser);
	if (simple_cmd != NULL || parser->error != NULL)
		return ((t_command *) simple_cmd);
	return (NULL);
}

void	parser_destroy_command(t_command *command)
{
	if (command == NULL)
		return ;
	if (command->node_type == T_SIMPLE_COMMAND)
		parser_destroy_simple_command((t_simple_command *) command);
	else if (command->node_type == T_SUBSHELL)
		parser_destroy_subshell((t_subshell *) command);
}
