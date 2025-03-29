#include "minishell.h"

int	builtin_env(int argc, char *argv[], char *envp[])
{
	(void)argv;
	if (argc > 1)
	{
		print_labeled_error("env", "too many arguments");
		return (EXIT_FAILURE);
	}
	sh_env_display((const char **) envp, ENV_PRINT_NONE);
	return (EXIT_SUCCESS);
}
