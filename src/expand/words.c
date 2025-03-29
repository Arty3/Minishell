#include "minishell.h"

t_wildcard_pos	*wildcard_pos_create(size_t index)
{
	t_wildcard_pos	*pos;

	pos = ft_calloc(1, sizeof(t_wildcard_pos));
	if (pos == NULL)
		return (NULL);
	pos->index = index;
	return (pos);
}

bool	expander_add_wildcard(t_expander *expander, const char *value)
{
	size_t			i;
	size_t			buf_len;
	t_wildcard_pos	*pos;

	i = 0;
	if (value == NULL)
		return (true);
	if (expander->buffer == NULL)
		buf_len = 0;
	else
		buf_len = ft_strlen(expander->buffer);
	while (value[i] != '\0')
	{
		if (value[i] != '*' && ++i)
			continue ;
		pos = wildcard_pos_create(buf_len + i);
		if (pos == NULL)
			return (false);
		ft_append_lst(&expander->wildcards, ft_init_lst(pos));
		++i;
	}
	return (true);
}

// Expand a single token into multiple tokens if applicable. E.g. if a token
// consists of a variable `ech$VAR` and $VAR expands into a separate token.
// A situation where that would be the case is if $VAR expands into `"o hello"`
// for example. We then perform string quote removal and end up with
// `echo hello`. Word splitting is performed and `hello` should now be
// considered an argument.
//
// If there's only one string resulting from the split, we leave the value in
// the buffer.
static
bool	expander_expand_word_split(t_expander *expander, const char *value,
	char *split[])
{
	size_t	i;

	i = 0;
	if (split[1] == NULL && !expander_add_wildcard(expander, split[0]))
		return (ft_free2d((void **) split), false);
	if (!ft_isspace(value[0]))
		expander->buffer = buffer_append_word(expander->buffer, split[i++]);
	if (split[i] == NULL)
		return (ft_free2d((void **) split), true);
	if (!expander_push_buffer(expander))
		return (ft_free2d((void **) split), false);
	while (split[i] != NULL && split[i + 1] != NULL)
	{
		if (!expander_add_wildcard(expander, split[i]))
			return (ft_free2d((void **) split), false);
		expander->buffer = buffer_append_word(expander->buffer, split[i]);
		if (expander->buffer == NULL || !expander_push_buffer(expander))
			return (ft_free2d((void **) split), false);
		++i;
	}
	if (split[i] != NULL && !expander_add_wildcard(expander, split[i]))
		return (ft_free2d((void **) split), false);
	if (split[i] != NULL && split[i + 1] == NULL)
		expander->buffer = buffer_append_word(expander->buffer, split[i]);
	return (ft_free2d((void **) split), expander->buffer != NULL);
}

// Splitting on space could probably be replaced with `IFS`
//
// We split up the value into individual chunks and then add it to the list
// depending on the context. If we're currently within quotes then it should
// become part of the whole word. If we're not within quotes then we apply all
// words up until the second to last one. The last one is left in the buffer
// since it's possible to have consecutive variable args.
bool	expander_expand_variable_args(t_expander *expander,
	const char *value)
{
	char	**split;

	if (value == NULL)
		return (true);
	if (expander->state == DOUBLE_QUOTE || expander->flags & NO_SPLITTING)
	{
		expander->buffer = buffer_append_word(expander->buffer, value);
		return (true);
	}
	if (expander->state != NO_QUOTE)
		return (true);
	split = ft_strsplit(value, " ");
	if (split == NULL)
		return (false);
	if (split[0] == NULL)
		return (ft_free2d((void **)split), true);
	return (expander_expand_word_split(expander, value, split));
}
