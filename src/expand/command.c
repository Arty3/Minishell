#include "minishell.h"

static
t_list	*expander_merge_command_tokens(t_simple_command *command,
	t_list *tokens, t_shell *shell)
{
	t_expander	expander;
	t_list		*argument_node;
	t_token		*argument;
	t_list		*expanded_tokens;

	expanded_tokens = tokens;
	argument_node = command->args;
	while (argument_node != NULL)
	{
		argument = (t_token *)argument_node->content;
		expander_init(&expander, QUOTE_REMOVAL | VARIABLE_EXPANSION
			| TILDE_EXPANSION | FILENAME_EXPANSION);
		expander.shell = shell;
		tokens = expander_expand_token(&expander, argument,
				(const char **) shell->envp);
		if (tokens != NULL)
			ft_append_lst(&expanded_tokens, tokens);
		expander_destroy(&expander);
		argument_node = argument_node->next;
	}
	return (expanded_tokens);
}

// NOTE: we set the command name to NULL to indicate the command is not to be
// executed but the exit status shall be set to 0.
static
void	expander_finalize_command_merge(t_simple_command *command,
	t_list *expanded_tokens)
{
	t_list	*token_node;

	token_node = expanded_tokens;
	lexer_destroy_token(command->name);
	if (token_node == NULL)
	{
		command->name = NULL;
		return ;
	}
	expanded_tokens = expanded_tokens->next;
	command->name = token_node->content;
	free(token_node);
	ft_clear_lst(&command->args, (void (*)(void *)) lexer_destroy_token);
	ft_append_lst(&command->args, expanded_tokens);
}

// NOTE: this only expands the command name and their respective arguments.
// No modifications are to be applied onto any of the redirections.
bool	expander_expand_command(t_command *command,
	t_shell *shell)
{
	t_expander	expander;
	t_list		*tokens;
	t_token		*cmd_name_token;

	if (command->node_type != T_SIMPLE_COMMAND)
		return (false);
	expander_init(&expander, QUOTE_REMOVAL | VARIABLE_EXPANSION
		| TILDE_EXPANSION | FILENAME_EXPANSION);
	expander.shell = shell;
	cmd_name_token = ((t_simple_command *)command)->name;
	tokens = NULL;
	if (cmd_name_token != NULL)
		tokens = expander_expand_token(&expander, cmd_name_token,
				(const char **) shell->envp);
	expander_destroy(&expander);
	tokens = expander_merge_command_tokens((t_simple_command *) command,
			tokens, shell);
	expander_finalize_command_merge((t_simple_command *) command, tokens);
	return (true);
}
