#include <fnmatch.h>

#include "minishell.h"

static
bool	expander_push_filename_path(t_expander *expander, char *path)
{
	t_token	*new_token;
	char	*new_path;

	new_path = ft_strdup(path);
	if (new_path == NULL)
		return (false);
	new_token = lexer_create_token(new_path);
	if (new_token == NULL)
		return (false);
	if (!expander_append_token(expander, new_token))
	{
		lexer_destroy_token(new_token);
		return (false);
	}
	return (true);
}

// If the token does not expand into a path then the token is to be
// remained the same.
bool	expander_expand_filename(t_expander *expander,
	t_token *expanded_token)
{
	t_list	*paths;
	t_list	*path;

	if (expander->wildcards == NULL || ~(expander->flags) & FILENAME_EXPANSION)
		return (false);
	paths = shell_glob(expander->wildcards, expanded_token->raw);
	if (paths == NULL)
		return (false);
	path = paths;
	while (path != NULL)
	{
		if (!expander_push_filename_path(expander, path->content))
			return (ft_clear_lst(&paths, free), false);
		path = path->next;
	}
	ft_clear_lst(&paths, free);
	return (true);
}
