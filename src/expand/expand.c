#include "minishell.h"

static
bool	expander_expand_quote(t_expander *expander, t_token *token,
	const char **__vars)
{
	if (expander->state == SINGLE_QUOTE)
		return (expander_expand_single_quote(expander, token));
	else if (expander->state == DOUBLE_QUOTE)
		return (expander_expand_double_quote(expander, token, __vars));
	return (expander_expand_no_quote(expander, token, __vars));
}

// NOTE: expander->buffer should point to NULL if it completed successfully
t_list	*expander_expand_token(t_expander *expander, t_token *token,
	const char **__vars)
{
	while (token->raw[expander->pos] != '\0')
	{
		if (expander->state == NO_QUOTE && is_quote(token->raw[expander->pos])
			&& expander->flags & QUOTE_REMOVAL)
		{
			expander->state = get_quote_type(token->raw[expander->pos]);
			expander->quote_pos = expander->pos++;
		}
		else if (!expander_expand_quote(expander, token, __vars))
			return (NULL);
	}
	if (expander->state != NO_QUOTE)
	{
		expander_destroy(expander);
		return (NULL);
	}
	expander_push_buffer(expander);
	return (expander->tokens);
}

void	expander_init(t_expander *expander, t_expander_flag flags)
{
	ft_memset(expander, 0, sizeof(t_expander));
	expander->flags = flags;
}

void	expander_destroy(t_expander *expander)
{
	if (expander->buffer != NULL)
	{
		free(expander->buffer);
		expander->buffer = NULL;
	}
	if (expander->wildcards != NULL)
	{
		ft_clear_lst(&expander->wildcards, free);
		expander->wildcards = NULL;
	}
}
