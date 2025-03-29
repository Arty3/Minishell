#include "minishell.h"

// https://pubs.opengroup.org/onlinepubs/9699919799/
// utilities/V3_chap02.html#tag_18_05_02
static
size_t	get_variable_name_length(const char *buf)
{
	size_t	i;

	i = 0;
	if (ft_isdigit(buf[0]))
		return (0);
	if (buf[0] == '?')
		return (1);
	while (buf[i] != '\0')
	{
		if (!ft_isalnum(buf[i]) && buf[i] != '_')
			break ;
		++i;
	}
	return (i);
}

static
char	*get_special_parameter_value(t_expander *expander, const char *key)
{
	if (expander->shell == NULL)
		return (NULL);
	if (key[0] == '?')
		return (ft_itoa(expander->shell->exit_status));
	return (NULL);
}

static
char	*get_parameter_value(t_expander *expander, const char *key,
	const char **__vars)
{
	char	*value;

	value = get_special_parameter_value(expander, key);
	if (value != NULL)
		return (value);
	value = (char *) sh_env_get_value((char **) __vars, key);
	if (value != NULL)
		value = ft_strdup(value);
	if (value == NULL)
		value = ft_strdup("");
	return (value);
}

bool	expander_expand_variable(t_expander *expander, t_token *token,
	const char **__vars)
{
	size_t	length;
	char	*key;
	char	*value;

	if (expander->state == NO_QUOTE
		&& is_quote(token->raw[expander->pos + 1]) && ++expander->pos)
		return (true);
	length = get_variable_name_length(token->raw + expander->pos + 1);
	if (length == 0)
	{
		expander->buffer = buffer_append_char(expander->buffer,
				token->raw[expander->pos++]);
		return (true);
	}
	key = ft_substr(token->raw, expander->pos + 1, length);
	if (key == NULL)
		return (false);
	value = get_parameter_value(expander, key, __vars);
	free(key);
	if (value == NULL)
		return (false);
	expander->pos += length + 1;
	if (!expander_expand_variable_args(expander, value))
		return (free(value), false);
	return (free(value), true);
}
