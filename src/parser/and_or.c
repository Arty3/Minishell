#include "minishell.h"

t_and_or	*parser_parse_and_or(t_parser *parser)
{
	t_pipeline	*pipeline;
	t_and_or	*and_or;

	and_or = ft_calloc(1, sizeof(t_and_or));
	if (and_or == NULL)
		return (NULL);
	and_or->node_type = T_AND_OR;
	parser_skip_linebreaks(parser);
	pipeline = parser_parse_pipeline(parser);
	if (pipeline == NULL)
		return (parser_destroy_and_or(and_or), NULL);
	and_or->pipeline = pipeline;
	return (and_or);
}

static
t_list	*parser_parse_and_or_loop(t_parser *parser, t_list *and_or_list,
	t_and_or *and_or)
{
	t_list	*and_or_list_node;
	t_token	*token;

	token = lexer_peek(parser->lexer);
	while (and_or != NULL && token != NULL
		&& (token->type == T_AND_IF || token->type == T_OR_IF))
	{
		token = lexer_pop(parser->lexer);
		and_or = parser_parse_and_or(parser);
		if (and_or == NULL)
			return (parser_set_error(parser,
					"missing expressions after `||' or `&&'"),
				parser_destroy_and_or_list(&and_or_list), NULL);
		and_or->type = token->type;
		and_or_list_node = ft_init_lst(and_or);
		if (and_or_list_node == NULL)
			return (parser_set_error(parser, "failed to allocate memory"),
				parser_destroy_and_or(and_or),
				parser_destroy_and_or_list(&and_or_list), NULL);
		ft_append_lst(&and_or_list, and_or_list_node);
		token = lexer_peek(parser->lexer);
	}
	return (and_or_list);
}

// A list of consecutive AND-OR operations.
// [... || <cmd> && <cmd> && ...]
t_list	*parser_parse_and_or_list(t_parser *parser)
{
	t_list		*and_or_list;
	t_list		*and_or_list_node;
	t_and_or	*and_or;

	and_or_list = NULL;
	and_or = parser_parse_and_or(parser);
	if (and_or == NULL)
		return (NULL);
	and_or_list_node = ft_init_lst(and_or);
	if (and_or_list_node == NULL)
	{
		parser_set_error(parser, "failed to allocate memory");
		parser_destroy_and_or(and_or);
		return (NULL);
	}
	ft_append_lst(&and_or_list, and_or_list_node);
	return (parser_parse_and_or_loop(parser, and_or_list, and_or));
}

void	parser_destroy_and_or(t_and_or *and_or)
{
	if (and_or == NULL)
		return ;
	if (and_or->pipeline != NULL)
		parser_destroy_pipeline(and_or->pipeline);
	free(and_or);
}

void	parser_destroy_and_or_list(t_list **and_or_list)
{
	if (and_or_list == NULL || *and_or_list == NULL)
		return ;
	ft_clear_lst(and_or_list,
		(void (*)(void *)) parser_destroy_and_or);
}
