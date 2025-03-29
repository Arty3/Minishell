#include "minishell.h"

int	get_exit_status(int stat_loc)
{
	int	status;

	status = EXIT_FAILURE;
	if (WIFEXITED(stat_loc))
		status = WEXITSTATUS(stat_loc);
	else if (WIFSIGNALED(stat_loc))
		status = 128 + WTERMSIG(stat_loc);
	else if (WIFSTOPPED(stat_loc))
		status = WSTOPSIG(stat_loc);
	return (status);
}

void	print_exit_signal(int signal)
{
	if (signal > 128)
		signal -= 128;
	if (signal == SIGQUIT)
		ft_write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	if (signal == SIGINT)
		ft_write(STDERR_FILENO, "\n", 1);
}

int	wait_for_child(pid_t child)
{
	int	status;

	if (waitpid(child, &status, 0) < 0)
		return (EXIT_FAILURE);
	if (WIFSIGNALED(status))
		print_exit_signal(status);
	status = get_exit_status(status);
	return (status);
}

static
int32_t	do_fork(t_shell *shell, t_cmd *cmd, int fds[2], bool async)
{
	int	status;

	cmd->pid = fork();
	status = EXIT_SUCCESS;
	if (cmd->pid == -1)
		return (print_error(ENOFORK), EXIT_FAILURE);
	if (!cmd->pid)
	{
		assign_shell_traps(SIG_TRAP_RESET);
		fd_bitmap_close(shell->fd, CLOSE_PRE_EXEC_CHILD);
		if (sh_redirect_io(shell, fds[0], fds[1]) == -1)
			status = EXIT_FAILURE;
		else if (execve(cmd->bin,
				(char *const *) cmd->argv, cmd->envp) != 0)
			status = EXIT_BIN_CANT_EXEC;
		status = handle_execve_error(cmd, errno);
		ft_free2d((void **) cmd->argv);
		destroy_exec_cmd(cmd);
		(destroy_shell(shell), exit(status));
	}
	if (!async)
		return (wait_for_child(cmd->pid));
	return (EXIT_SUCCESS);
}

int	execute_cmd(
	t_shell *shell,
	t_cmd *cmd,
	int fds[2], bool async)
{
	if (cmd->is_builtin)
		return (do_builtin_cmd(shell, cmd, fds, async));
	return (do_fork(shell, cmd, fds, async));
}
