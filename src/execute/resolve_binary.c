#include "minishell.h"

static
bool	is_valid_bin(const char *path)
{
	if (is_directory(path))
		return (false);
	if (!is_executable(path))
		return (false);
	return (true);
}

static
char	*find_bin_path(const char *bin, const char *env_path)
{
	char	**paths;
	char	*joined_path;
	size_t	i;

	i = 0;
	if (env_path == NULL)
		return (NULL);
	paths = ft_strsplit(env_path, ":");
	if (paths == NULL)
		return (NULL);
	while (paths[i] != NULL)
	{
		joined_path = path_join(paths[i], (char *) bin, false);
		if (joined_path == NULL)
			return (ft_free2d((void **) paths), NULL);
		if (is_valid_bin(joined_path))
			return (ft_free2d((void **) paths), joined_path);
		free(joined_path);
		++i;
	}
	ft_free2d((void **) paths);
	return (NULL);
}

int	resolve_binary(const char *bin, char **out, const char *envp[])
{
	const char	*env_path = sh_env_get_value((char **) envp, "PATH");
	char		*path;

	if (is_builtin(bin) || ft_strchr(bin, '/') != NULL || env_path == NULL)
	{
		*out = ft_strdup(bin);
		if (*out == NULL)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	path = find_bin_path(bin, env_path);
	if (path == NULL)
	{
		print_labeled_error(bin, "command not found");
		return (EXIT_BIN_NOT_FOUND);
	}
	*out = path;
	return (EXIT_SUCCESS);
}
