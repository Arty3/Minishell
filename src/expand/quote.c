#include "minishell.h"

static
bool	expander_explicit_null_argument(t_expander *expander)
{
	if (expander->pos < 2 || expander->quote_pos != expander->pos - 2)
		return (true);
	expander->buffer = buffer_append_word(expander->buffer, "");
	return (expander->buffer != NULL);
}

// Single quoted strings are left untouched.
bool	expander_expand_single_quote(t_expander *expander, t_token *token)
{
	if (token->raw[expander->pos] == '\'')
	{
		++expander->pos;
		if (!expander_explicit_null_argument(expander))
			return (false);
		expander->state = NO_QUOTE;
	}
	else
		expander->buffer = buffer_append_char(expander->buffer,
				token->raw[expander->pos++]);
	return (true);
}

bool	expander_expand_double_quote(t_expander *expander, t_token *token,
	const char **__vars)
{
	if (token->raw[expander->pos] == '\"')
	{
		++expander->pos;
		if (!expander_explicit_null_argument(expander))
			return (false);
		expander->state = NO_QUOTE;
		return (true);
	}
	if (token->raw[expander->pos] == '$'
		&& expander->flags & VARIABLE_EXPANSION)
		return (expander_expand_variable(expander, token, __vars));
	if (should_escape(expander, token))
		++expander->pos;
	expander->buffer = buffer_append_char(expander->buffer,
			token->raw[expander->pos++]);
	return (expander->buffer != NULL);
}

// https://www.gnu.org/software/bash/manual/html_node/Tilde-Expansion.html
//
// https://pubs.opengroup.org/onlinepubs/9699919799/
// utilities/V3_chap02.html#tag_18_06_01
//
// NOTE: the function currently does not handle logins as we're not permitted
// to use such functions (getpwnam).
static
bool	expander_expand_tilde(t_expander *expander, t_token *token)
{
	const char	*home_path;

	if (expander->shell == NULL)
		return (false);
	if ((expander->pos != 0
			&& token->raw[expander->pos - 1] != '='))
		return (false);
	if (token->raw[expander->pos + 1] != '\0'
		&& token->raw[expander->pos + 1] != '/')
		return (false);
	home_path = sh_env_get_value(expander->shell->envp, "HOME");
	if (home_path == NULL)
		home_path = expander->shell->default_home;
	if (home_path != NULL)
		expander->buffer = buffer_append_word(expander->buffer, home_path);
	else
		return (false);
	++expander->pos;
	return (true);
}

bool	expander_expand_no_quote(t_expander *expander, t_token *token,
	const char **__vars)
{
	if (ft_isspace(token->raw[expander->pos])
		&& ~(expander->flags) & NO_SPLITTING
		&& ++expander->pos)
		return (expander_push_buffer(expander));
	if (token->raw[expander->pos] == '$'
		&& expander->flags & VARIABLE_EXPANSION)
		return (expander_expand_variable(expander, token, __vars));
	if (token->raw[expander->pos] == '~'
		&& expander->flags & TILDE_EXPANSION
		&& expander_expand_tilde(expander, token))
		return (true);
	if (token->raw[expander->pos] == '*'
		&& !expander_add_wildcard(expander, "*"))
		return (false);
	if (should_escape(expander, token))
		++expander->pos;
	expander->buffer = buffer_append_char(expander->buffer,
			token->raw[expander->pos++]);
	return (true);
}
