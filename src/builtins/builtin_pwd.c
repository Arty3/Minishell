#include "minishell.h"

int	builtin_pwd(char *envp[])
{
	const char	*pwd;
	char		cwd[PATH_MAX];

	if (ft_getcwd(cwd, PATH_MAX) != NULL)
		ft_write(STDOUT_FILENO, cwd, ft_strlen(cwd));
	else
	{
		pwd = sh_env_get_value(envp, "PWD");
		if (pwd == NULL)
		{
			print_labeled_error("pwd", "failed to get pwd");
			return (EXIT_FAILURE);
		}
		ft_write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	}
	ft_write(STDOUT_FILENO, "\n", 1);
	return (EXIT_SUCCESS);
}
