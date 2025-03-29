#include "minishell.h"

static
bool	is_operator(int c)
{
	return (c == '&' || c == '|' || c == '<' || c == '>' || c == '\n'
		|| c == '(' || c == ')');
}

static
bool	ft_isblank(int c)
{
	return (c == ' ' || c == '\t');
}

ssize_t	get_word_length(t_lexer *lexer)
{
	char	quote;
	char	*buf;
	size_t	i;

	i = 0;
	quote = '\0';
	buf = lexer->buf + lexer->pos;
	while (buf[i] != '\0')
	{
		if (buf[i] == '\\' && buf[i + 1] != '\0')
			++i;
		else if (is_quote(buf[i]))
		{
			if (quote != '\0' && buf[i] == quote)
				quote = '\0';
			else if (quote == '\0')
				quote = buf[i];
		}
		else if ((is_operator(buf[i]) || ft_isblank(buf[i])) && quote == '\0')
			break ;
		++i;
	}
	if (quote != '\0')
		return (-1);
	return (i);
}
