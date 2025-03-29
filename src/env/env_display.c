#include "env.h"
#include "utils.h"

static
void	sh_env_display_export(const char *var, int flags)
{
	size_t	name_len;

	(void)flags;
	name_len = ft_strcspn(var, "=");
	ft_write(STDOUT_FILENO, "declare -x ", 11);
	ft_write(STDOUT_FILENO, var, name_len);
	if (var[name_len] != '\0')
	{
		ft_write(STDOUT_FILENO, "=\"", 2);
		ft_write(STDOUT_FILENO, var + name_len + 1,
			ft_strlen(var + name_len + 1));
		ft_write(STDOUT_FILENO, "\"", 1);
	}
	ft_write(STDOUT_FILENO, "\n", 1);
}

void	sh_env_display(const char *envp[], int flags)
{
	size_t	name_len;
	size_t	i;

	i = 0;
	while (envp[i] != NULL)
	{
		name_len = ft_strcspn(envp[i], "=");
		if (flags & ENV_PRINT_EXPORT)
			sh_env_display_export(envp[i], flags);
		else if (envp[i][name_len] != '\0')
		{
			ft_write(STDOUT_FILENO, envp[i], ft_strlen(envp[i]));
			ft_write(STDOUT_FILENO, "\n", 1);
		}
		++i;
	}
}

bool	sh_env_display_sorted(const char *envp[], int flags)
{
	char	**sorted;

	sorted = create_sorted_array((char **) envp, ft_strcmp);
	if (sorted == NULL)
		return (false);
	sh_env_display((const char **) sorted, flags);
	ft_free2d((void **) sorted);
	return (true);
}
