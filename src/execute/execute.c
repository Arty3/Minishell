#include "minishell.h"

static
int	execute_with_shell(t_shell *shell, t_node *node)
{
	int	status;

	if (node->node_type != T_PROGRAM)
		return (false);
	status = traverse_heredoc_node(shell, node);
	if (status != EXIT_SUCCESS)
	{
		shell->exit_status = status;
		return (true);
	}
	return (execute_and_or_list(shell, ((t_program *) node)->and_or_list));
}

static
bool	handle_parser_error(t_shell *shell, t_parser *parser)
{
	if (parser->error == NULL)
		return (false);
	print_labeled_error("syntax error", parser->error);
	shell->exit_status = EXIT_PARSER_ERROR;
	return (true);
}

bool	execute_from_lexer(t_shell *shell, t_lexer *lexer)
{
	bool	result;

	shell->parser = parser_create(lexer);
	if (shell->parser == NULL)
		return (false);
	shell->program = parser_parse(shell->parser);
	if (handle_parser_error(shell, shell->parser))
	{
		parser_destroy_program(shell->program);
		parser_destroy(shell->parser);
		shell->program = NULL;
		shell->parser = NULL;
		return (true);
	}
	result = execute_with_shell(shell, (t_node *) shell->program);
	parser_destroy_program(shell->program);
	parser_destroy(shell->parser);
	shell->program = NULL;
	shell->parser = NULL;
	return (result);
}
