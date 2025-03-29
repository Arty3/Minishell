#include "minishell.h"

t_lexer	*lexer_create(const char *str)
{
	t_lexer	*lexer;

	lexer = ft_calloc(1, sizeof(t_lexer));
	if (lexer == NULL)
		return (NULL);
	lexer->buf = ft_strdup(str);
	if (lexer->buf == NULL)
	{
		lexer_destroy(lexer);
		return (NULL);
	}
	return (lexer);
}

void	lexer_destroy(t_lexer *lexer)
{
	if (lexer == NULL)
		return ;
	if (lexer->buf != NULL)
		free(lexer->buf);
	if (lexer->tokens != NULL)
		ft_clear_lst(&lexer->tokens,
			(void (*)(void *)) lexer_destroy_token);
	if (lexer->error != NULL)
		free(lexer->error);
	free(lexer);
}

static
ssize_t	get_token_length(t_lexer *lexer)
{
	ssize_t	word_len;
	char	*buf;

	buf = lexer->buf + lexer->pos;
	if (buf[0] == '&' && buf[1] != '&')
	{
		lexer_set_error(lexer, "unexpected character - missing ampersand");
		return (-1);
	}
	if (!ft_strncmp(buf, "&&", 2) || !ft_strncmp(buf, "||", 2))
		return (2);
	if (!ft_strncmp(buf, "<<", 2) || !ft_strncmp(buf, ">>", 2))
		return (2);
	if (!ft_strncmp(buf, "(", 1) || !ft_strncmp(buf, ")", 1)
		|| !ft_strncmp(buf, "<", 1) || !ft_strncmp(buf, ">", 1)
		|| !ft_strncmp(buf, "|", 1) || !ft_strncmp(buf, "\n", 1))
		return (1);
	word_len = get_word_length(lexer);
	if (word_len == -1)
		lexer_set_error(lexer, "unexpected end of line - missing quote");
	return (word_len);
}

void	lexer_set_error(t_lexer *lexer, const char *message)
{
	if (message != NULL && lexer->error == NULL)
		lexer->error = ft_strdup(message);
	else if (message == NULL)
	{
		if (lexer->error != NULL)
			free(lexer->error);
		lexer->error = NULL;
	}
}

bool	lexer_tokenize(t_lexer *lexer)
{
	ssize_t		token_length;
	char		*sub;

	token_length = 0;
	while (lexer->buf[lexer->pos] != '\0')
	{
		token_length = get_token_length(lexer);
		if (token_length == -1)
			return (false);
		if (token_length > 0)
		{
			sub = ft_substr(lexer->buf, lexer->pos, token_length);
			if (sub == NULL || !lexer_append_token(lexer, sub))
				return (false);
			lexer->pos += token_length;
		}
		else if (ft_isspace(lexer->buf[lexer->pos]) && ++lexer->pos)
			continue ;
	}
	lexer->current_token = lexer->tokens;
	return (lexer->error == NULL);
}
