#include "minishell.h"

char	*get_base_path(const char *pattern)
{
	char		cwd[PATH_MAX];
	const char	*ret = getcwd(cwd, PATH_MAX);
	char		*base_path;

	if (ret == NULL)
		return (NULL);
	if (is_path_relative(pattern))
		base_path = ft_strdup(cwd);
	else
		base_path = strdup("");
	return (base_path);
}

bool	is_glob_path_valid(const char *path, t_glob *glob)
{
	char	resolved_path[PATH_MAX];

	if (frpath(path, (char *restrict) &resolved_path) == NULL)
		return (false);
	if (glob->next_segment != NULL && !is_directory(resolved_path))
		return (false);
	return (true);
}
