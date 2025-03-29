#include "minishell.h"

static
bool	parse_command_name(t_parser *parser, t_simple_command *cmd)
{
	t_token	*token;

	token = lexer_peek(parser->lexer);
	if (token == NULL || token->type != T_WORD)
		return (false);
	cmd->name = lexer_copy_token(token);
	lexer_pop(parser->lexer);
	if (cmd->name == NULL)
	{
		parser_set_error(parser, "failed to allocate memory");
		return (false);
	}
	return (true);
}

static
bool	parse_command_suffix(t_parser *parser, t_simple_command *cmd)
{
	t_redir	*redir;
	t_token	*token;
	t_list	*redir_list;

	redir = parser_parse_redir(parser);
	if (redir != NULL)
	{
		redir_list = ft_init_lst(redir);
		if (redir_list == NULL)
			return (parser_set_error(parser, "failed to allocate memory"),
				parser_destroy_redir(redir), false);
		ft_append_lst(&cmd->redirs, redir_list);
		return (true);
	}
	token = lexer_peek(parser->lexer);
	if (token != NULL && token->type == T_WORD)
	{
		redir_list = ft_init_lst(lexer_copy_token(token));
		if (redir_list == NULL)
			return (parser_set_error(parser, "failed to allocate memory"),
				false);
		ft_append_lst(&cmd->args, redir_list);
		return (lexer_pop(parser->lexer), true);
	}
	return (false);
}

static
bool	parse_command_prefix(t_parser *parser, t_simple_command *cmd)
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
		ft_append_lst(&cmd->redirs, redir_list);
		return (true);
	}
	return (false);
}

// The order in which we store and apply the redirects shouldn't matter as we
// apply the redirects prior to actually executing the command.
t_simple_command	*parser_parse_simple_command(t_parser *parser)
{
	t_simple_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_simple_command));
	if (cmd == NULL)
		return (NULL);
	cmd->node_type = T_SIMPLE_COMMAND;
	while (parse_command_prefix(parser, cmd))
		continue ;
	if (!parse_command_name(parser, cmd) && cmd->redirs == NULL)
		return (parser_destroy_simple_command(cmd), NULL);
	while (parse_command_suffix(parser, cmd))
		continue ;
	return (cmd);
}

void	parser_destroy_simple_command(t_simple_command *command)
{
	if (command == NULL)
		return ;
	if (command->name != NULL)
		lexer_destroy_token(command->name);
	if (command->redirs != NULL)
		ft_clear_lst(&command->redirs,
			(void (*)(void *)) parser_destroy_redir);
	if (command->args != NULL)
		ft_clear_lst(&command->args,
			(void (*)(void *)) lexer_destroy_token);
	free(command);
}
