#include "minishell.h"

t_quote_type	get_quote_type(int32_t c)
{
	if (c == '\'')
		return (SINGLE_QUOTE);
	if (c == '\"')
		return (DOUBLE_QUOTE);
	return (NO_QUOTE);
}

// https://www.gnu.org/software/bash/manual/html_node/Double-Quotes.html
#define DOUBLE_QUOTE_ESCAPED_CHARSET "$`\"\\"

inline bool	should_escape(t_expander *expander, t_token *token)
{
	if (~(expander->flags & HEREDOC) && expander->state == DOUBLE_QUOTE
		&& token->raw[expander->pos] == '\\'
		&& ft_strchr(DOUBLE_QUOTE_ESCAPED_CHARSET,
			token->raw[expander->pos + 1]) != NULL)
		return (true);
	if ((expander->state == NO_QUOTE || expander->flags & HEREDOC)
		&& token->raw[expander->pos] == '\\'
		&& token->raw[expander->pos + 1] != '\0')
		return (true);
	return (false);
}
