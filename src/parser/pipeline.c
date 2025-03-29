#include "minishell.h"

static
t_pipeline	*parser_parse_pipeline_loop(t_parser *parser, t_pipeline *pipeline)
{
	t_command	*cmd;
	t_list		*cmd_node;
	t_token		*token;

	token = lexer_peek(parser->lexer);
	while (token != NULL && token->type == T_OR)
	{
		lexer_pop(parser->lexer);
		parser_skip_linebreaks(parser);
		cmd = parser_parse_command(parser);
		if (cmd == NULL)
			return (parser_set_error(parser, "expected a command"),
				parser_destroy_pipeline(pipeline), NULL);
		cmd_node = ft_init_lst(cmd);
		if (cmd_node == NULL)
			return (parser_set_error(parser, "failed memory allocation"),
				parser_destroy_command(cmd), parser_destroy_pipeline(pipeline),
				NULL);
		ft_append_lst(&pipeline->commands, cmd_node);
		token = lexer_peek(parser->lexer);
	}
	return (pipeline);
}

// [... | <cmd> | <cmd> | ...]
t_pipeline	*parser_parse_pipeline(t_parser *parser)
{
	t_command	*cmd;
	t_list		*cmd_node;
	t_pipeline	*pipeline;

	cmd = parser_parse_command(parser);
	if (cmd == NULL)
		return (NULL);
	pipeline = ft_calloc(1, sizeof(t_pipeline));
	if (pipeline == NULL)
		return (NULL);
	pipeline->node_type = T_PIPELINE;
	cmd_node = ft_init_lst(cmd);
	if (cmd_node == NULL)
		return (NULL);
	ft_append_lst(&pipeline->commands, cmd_node);
	return (parser_parse_pipeline_loop(parser, pipeline));
}

void	parser_destroy_pipeline(t_pipeline *pipeline)
{
	if (pipeline == NULL)
		return ;
	if (pipeline->commands != NULL)
		ft_clear_lst(&pipeline->commands,
			(void (*)(void *)) parser_destroy_command);
	free(pipeline);
}
