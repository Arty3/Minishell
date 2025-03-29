#include "minishell.h"

static
char	*get_working_directory(char *caller_name, char cwd[PATH_MAX])
{
	char	err_msg[MAX_ERROR_LEN];

	if (ft_getcwd(cwd, PATH_MAX) != NULL)
		return (cwd);
	ft_memset(err_msg, 0, sizeof(err_msg));
	ft_strlcat(err_msg, "error retrieving current directory: ",
		MAX_ERROR_LEN);
	ft_strlcat(err_msg, "getcwd: cannot access parent directories: ",
		MAX_ERROR_LEN);
	ft_strlcat(err_msg, strerror(errno), MAX_ERROR_LEN);
	print_labeled_error(caller_name, err_msg);
	return (NULL);
}

static
bool	update_env(char **envp[])
{
	char		cwd[PATH_MAX];
	const char	*prev_pwd;

	prev_pwd = sh_env_get_value(*envp, "PWD");
	if (!prev_pwd)
		prev_pwd = "";
	if (!sh_env_set_value(envp, "OLDPWD", prev_pwd))
		return (false);
	if (get_working_directory("cd", cwd) == NULL)
	{
		sh_env_append(envp, "PWD", "/.");
		return (false);
	}
	if (!sh_env_set_value(envp, "PWD", cwd))
		return (false);
	return (true);
}

static
char	*get_chdir_arg(int argc, char *argv[], char *envp[])
{
	const char	*path;

	path = NULL;
	if (argc == 1)
	{
		path = sh_env_get_value(envp, "HOME");
		if (!path)
			return (print_labeled_error("cd", "HOME not set"), NULL);
	}
	else if (ft_strequals(argv[1], "-"))
	{
		path = sh_env_get_value(envp, "OLDPWD");
		if (!path)
			return (print_labeled_error("cd", "OLDPWD not set"), NULL);
	}
	if (path == NULL)
		path = argv[1];
	if (path == NULL)
		return (print_labeled_error("cd", "failed to change dir"), NULL);
	return ((char *) path);
}

static
bool	safe_chdir(const char *path)
{
	char	msg[MAX_ERROR_LEN];

	if (chdir(path) == -1)
	{
		ft_memset(msg, 0, sizeof(msg));
		ft_strlcat(msg, path, MAX_ERROR_LEN);
		ft_strlcat(msg, ": ", MAX_ERROR_LEN);
		ft_strlcat(msg, strerror(errno), MAX_ERROR_LEN);
		print_labeled_error("cd", msg);
		return (false);
	}
	return (true);
}

int	builtin_cd(int argc, char *argv[], t_shell *shell)
{
	char	*path;

	if (argc > 2)
	{
		print_labeled_error("cd", "too many arguments");
		return (EXIT_FAILURE);
	}
	path = get_chdir_arg(argc, argv, shell->envp);
	if (path == NULL)
		return (EXIT_FAILURE);
	if (!safe_chdir(path))
		return (EXIT_FAILURE);
	if (ft_strequals(argv[1], "-"))
	{
		ft_write(STDOUT_FILENO, path, ft_strlen(path));
		ft_write(STDOUT_FILENO, "\n", 1);
	}
	if (!update_env(&shell->envp))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
