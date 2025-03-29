#include "env.h"
#include "utils.h"

bool	sh_env_append(char **envp[], const char *name, const char *append)
{
	const char	*value;
	char		*joined_value;

	value = sh_env_get_value(*envp, name);
	if (value == NULL)
		return (sh_env_set_value(envp, name, append));
	joined_value = ft_strjoin(value, append);
	if (joined_value == NULL)
		return (false);
	if (!sh_env_set_value(envp, name, joined_value))
	{
		free(joined_value);
		return (false);
	}
	free(joined_value);
	return (true);
}
