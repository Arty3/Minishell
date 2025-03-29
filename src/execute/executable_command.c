#include "minishell.h"

t_cmd	*init_exec_cmd(char *bin, char *argv[], char *envp[])
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (cmd == NULL)
		return (NULL);
	cmd->bin = bin;
	if (cmd->bin == NULL)
	{
		destroy_exec_cmd(cmd);
		return (NULL);
	}
	cmd->argv = (const char **) argv;
	cmd->envp = envp;
	cmd->argc = get_array_size(argv);
	cmd->is_builtin = is_builtin(bin);
	cmd->pid = -1;
	return (cmd);
}

void	destroy_exec_cmd(t_cmd *exec_cmd)
{
	if (exec_cmd == NULL)
		return ;
	if (exec_cmd->bin != NULL)
		free((char *) exec_cmd->bin);
	free(exec_cmd);
}

pid_t	execute_executable_cmd(t_shell *shell, char *argv[], int pipe_fds[2],
	bool async)
{
	t_cmd	*exec_cmd;
	char	*bin;
	int		exit_status;
	pid_t	pid;

	exit_status = resolve_binary(argv[0], &bin, (const char **) shell->envp);
	if (exit_status != EXIT_SUCCESS)
	{
		shell->exit_status = exit_status;
		return (-1);
	}
	exec_cmd = init_exec_cmd(bin, argv, shell->envp);
	if (exec_cmd == NULL)
	{
		print_labeled_error("error", strerror(ENOMEM));
		destroy_exec_cmd(exec_cmd);
		return (-1);
	}
	shell->exit_status = execute_cmd(shell, exec_cmd, pipe_fds, async);
	pid = exec_cmd->pid;
	destroy_exec_cmd(exec_cmd);
	return (pid);
}
