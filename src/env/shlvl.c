#include "minishell.h"

static
size_t	find_new_shlvl(char **envp[])
{
	const char	*raw_shlvl = sh_env_get_value(*envp, "SHLVL");
	intmax_t	prev_shlvl;
	size_t		shlvl;

	if (raw_shlvl == NULL || *raw_shlvl == '\0')
		shlvl = 1;
	else
	{
		if (!is_valid_number(raw_shlvl, &prev_shlvl))
			shlvl = 1;
		else
			shlvl = prev_shlvl + 1;
		if (prev_shlvl <= 0)
			shlvl = 0;
		if (prev_shlvl >= MAX_SHLVL || shlvl >= MAX_SHLVL)
		{
			print_labeled_error("warning", "shell level too high,"
				" resetting to 1");
			shlvl = 1;
		}
	}
	return (shlvl);
}

bool	env_adjust_shlvl(char **envp[])
{
	size_t	shlvl;
	char	*new_shlvl;
	bool	result;

	shlvl = find_new_shlvl(envp);
	new_shlvl = ft_itoa(shlvl);
	if (new_shlvl == NULL)
		return (false);
	result = sh_env_set_value(envp, "SHLVL", new_shlvl);
	free(new_shlvl);
	return (result);
}
