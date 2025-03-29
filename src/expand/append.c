#include "minishell.h"

char	*buffer_append_char(char *buffer, int32_t c)
{
	char	*temp;

	if (buffer == NULL)
		return (ft_strdup((char []){c, '\0'}));
	temp = buffer;
	buffer = ft_strjoin(buffer, (char []){c, '\0'});
	free(temp);
	return (buffer);
}

char	*buffer_append_word(char *buffer, const char *word)
{
	char	*temp;

	if (buffer == NULL)
		return (ft_strdup(word));
	temp = buffer;
	if (word == NULL)
		return (free(buffer), NULL);
	buffer = ft_strjoin(buffer, word);
	free(temp);
	return (buffer);
}

bool	expander_append_token(t_expander *expander, t_token *token)
{
	t_list	*new_token_node;

	new_token_node = ft_init_lst(token);
	if (new_token_node == NULL)
		return (false);
	ft_append_lst(&expander->tokens, new_token_node);
	return (true);
}

bool	expander_push_buffer(t_expander *expander)
{
	t_token	*new_token;

	if (expander->buffer == NULL)
		return (true);
	new_token = lexer_create_token(expander->buffer);
	if (new_token == NULL)
		return (false);
	if (expander_expand_filename(expander, new_token))
	{
		lexer_destroy_token(new_token);
		expander->buffer = NULL;
		ft_clear_lst(&expander->wildcards, free);
		return (true);
	}
	if (!expander_append_token(expander, new_token))
	{
		lexer_destroy_token(new_token);
		return (false);
	}
	expander->buffer = NULL;
	ft_clear_lst(&expander->wildcards, free);
	return (true);
}
