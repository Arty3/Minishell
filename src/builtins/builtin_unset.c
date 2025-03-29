#include "minishell.h"

static
bool	unset_var(const char *name, char **envp[])
{
	if (is_valid_identifier(name) && !sh_env_unset(envp, name))
	{
		print_labeled_error("unset", "failed to unset variable");
		return (false);
	}
	return (true);
}

int	builtin_unset(int argc, char *argv[], t_shell *shell)
{
	size_t	i;
	int		exit_status;

	(void)argc;
	i = 1;
	exit_status = EXIT_SUCCESS;
	while (argv[i])
	{
		if (!unset_var(argv[i], &shell->envp) != 0)
			exit_status = EXIT_FAILURE;
		++i;
	}
	return (exit_status);
}
