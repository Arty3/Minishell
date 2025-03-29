#include "minishell.h"

// As for "normal" redirs (T_LESS, T_GREAT & T_DGREAT) if the raw value doesn't
// expand into a value then the original is retained. E.g. if the redir name is
// `$VAR` but `VAR` does not have a value it would then retain `$VAR` as its
// name but would be considered ambigious.
// That is, this rule only applies if ALL the tokens in the name do NOT resolve
// to a variable value. E.g. if we have two variables, `VAR` and `VAR2` and only
// VAR2 resolves to a value then that would not be considered ambigious.
//
// ```bash
// < $FAKE_VAR
// bash: $FAKE_VAR: ambiguous redirect
// ```
//
// However, note that the following should still execute the first and last
// command respectively.
//
// ```bash
// echo 123 && echo 456 < $FAKE || echo 321
// ```
char	*expander_expand_redir_name(t_redir *redir, const char **__vars)
{
	t_expander	expander;
	t_list		*tokens;
	char		*name;

	expander_init(&expander, QUOTE_REMOVAL);
	if (redir->type != T_DLESS)
		expander.flags = QUOTE_REMOVAL | VARIABLE_EXPANSION
			| TILDE_EXPANSION | FILENAME_EXPANSION;
	tokens = expander_expand_token(&expander, redir->name, __vars);
	expander_destroy(&expander);
	if (tokens == NULL || tokens->next != NULL)
	{
		ft_clear_lst(&tokens, (void (*)(void *)) lexer_destroy_token);
		return (NULL);
	}
	name = ft_strdup(((t_token *) tokens->content)->raw);
	ft_clear_lst(&tokens, (void (*)(void *)) lexer_destroy_token);
	return (name);
}

char	*expander_expand_heredoc(t_redir *redir, t_shell *shell)
{
	t_expander	expander;
	t_list		*tokens;
	char		*content;
	t_token		*token;

	if (redir == NULL || redir->value == NULL)
		return (NULL);
	expander_init(&expander, NO_SPLITTING | HEREDOC);
	if (~(redir->name->flags) & F_QUOTED && ~(redir->name->flags) & F_ESCAPED)
		expander.flags |= VARIABLE_EXPANSION;
	expander.shell = shell;
	content = ft_strdup(redir->value);
	if (content == NULL)
		return (NULL);
	token = lexer_create_token(content);
	if (token == NULL)
		return (NULL);
	tokens = expander_expand_token(&expander, token,
			(const char **) shell->envp);
	expander_destroy(&expander);
	if (tokens == NULL)
		return (lexer_destroy_token(token), NULL);
	content = ft_strdup(((t_token *)tokens->content)->raw);
	ft_clear_lst(&tokens, (void (*)(void *)) lexer_destroy_token);
	return (lexer_destroy_token(token), content);
}
