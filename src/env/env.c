#include "env.h"
#include "utils.h"

static
ssize_t	sh_env_get_var_index(char *envp[], const char *name)
{
	const size_t	name_len = ft_strcspn(name, "=");
	size_t			var_name_len;
	size_t			i;

	i = 0;
	while (envp[i] != NULL)
	{
		var_name_len = ft_strcspn(envp[i], "=");
		if (!ft_strncmp(envp[i], name, var_name_len)
			&& !ft_strncmp(envp[i], name, name_len))
			return (i);
		++i;
	}
	return (-1);
}

const char	*sh_env_get_value(char *envp[], const char *name)
{
	ssize_t	var_index;
	char	*equals;

	var_index = sh_env_get_var_index(envp, name);
	if (var_index == -1)
		return (NULL);
	equals = ft_strchr(envp[var_index], '=');
	if (equals != NULL)
		return ((const char *) equals + 1);
	return (NULL);
}

static
bool	sh_env_set_internal(char **envp[], const char *name,
	const char *value, size_t index)
{
	const size_t	name_len = ft_strcspn(name, "=");
	char			*new_raw;
	size_t			len;

	len = name_len;
	if (value != NULL)
		len += ft_strlen(value);
	new_raw = ft_calloc(len + (int)(value != NULL) + 1, sizeof(char));
	if (new_raw == NULL)
		return (false);
	ft_memcpy(new_raw, name, name_len);
	if (value != NULL)
	{
		ft_memcpy(new_raw + name_len, "=", 1);
		ft_memcpy(new_raw + name_len + 1, value, ft_strlen(value));
	}
	(*envp)[index] = new_raw;
	return (true);
}

bool	sh_env_set_value(char **envp[], const char *name,
	const char *value)
{
	const size_t	envp_size = get_array_size(*envp);
	ssize_t			var_index;

	var_index = sh_env_get_var_index(*envp, name);
	if (var_index == -1)
	{
		*envp = ft_realloc(*envp, envp_size * sizeof(char *),
				(envp_size + 2) * sizeof(char *));
		if (*envp == NULL)
			return (false);
		var_index = envp_size;
		(*envp)[envp_size + 1] = NULL;
	}
	else
		free((*envp)[var_index]);
	return (sh_env_set_internal(envp, name, value, var_index));
}

bool	sh_env_unset(char **envp[], const char *name)
{
	const size_t	envp_size = get_array_size(*envp);
	const ssize_t	var_index = sh_env_get_var_index(*envp, name);
	char			**new_envp;

	if (var_index == -1)
		return (true);
	new_envp = ft_calloc(envp_size, sizeof(char *));
	if (new_envp == NULL)
		return (false);
	ft_memcpy(new_envp, *envp, var_index * sizeof(char *));
	ft_memcpy(new_envp + var_index, *envp + var_index + 1,
		(envp_size - var_index) * sizeof(char *));
	free((*envp)[var_index]);
	free(*envp);
	*envp = new_envp;
	return (true);
}
