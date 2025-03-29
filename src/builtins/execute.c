#include "minishell.h"

static int	execute_builtin(
		t_shell *shell,
		t_builtin_type type,
		const t_cmd *cmd)
{
	if (type == CD)
		return (builtin_cd(cmd->argc, (char **) cmd->argv, shell));
	else if (type == PWD)
		return (builtin_pwd(cmd->envp));
	else if (type == ENV)
		return (builtin_env(cmd->argc, (char **) cmd->argv, cmd->envp));
	else if (type == ECHO)
		return (builtin_echo(cmd->argc, (char **) cmd->argv, cmd->envp));
	else if (type == EXIT)
		return (builtin_exit((t_cmd *) cmd, shell));
	else if (type == UNSET)
		return (builtin_unset(cmd->argc, (char **) cmd->argv, shell));
	else if (type == EXPORT)
		return (builtin_export(cmd->argc, (char **) cmd->argv, shell));
	return (EXIT_FAILURE);
}

static int	std_execution(
		t_shell *shell,
		const t_cmd *cmd,
		t_builtin_type type,
		int fds[2])
{
	int	old_stdin;
	int	old_stdout;
	int	status;

	old_stdin = dup(STDIN_FILENO);
	old_stdout = dup(STDOUT_FILENO);
	fd_bitmap_close(shell->fd, CLOSE_PRE_EXEC_CHILD);
	if (old_stdin == -1 || old_stdout == -1)
		return (EXIT_FAILURE);
	if (sh_redirect_io(shell, fds[0], fds[1]) == -1)
		return (EXIT_FAILURE);
	fd_bitmap_set_fd(&shell->fd, old_stdin, CLOSE_IN_CHILD);
	fd_bitmap_set_fd(&shell->fd, old_stdout, CLOSE_IN_CHILD);
	status = execute_builtin(shell, type, cmd);
	if (sh_redirect_io(shell, old_stdin, old_stdout) == -1)
		return (EXIT_FAILURE);
	return (status);
}

static int	async_execution(
		t_shell *shell,
		t_cmd *cmd,
		t_builtin_type type,
		int fds[2])
{
	int	status;

	cmd->pid = fork();
	if (cmd->pid == -1)
	{
		print_error(ENOFORK);
		return (EXIT_FAILURE);
	}
	if (!cmd->pid)
	{
		assign_shell_traps(SIG_TRAP_RESET);
		if (sh_redirect_io(shell, fds[0], fds[1]) == -1)
			status = EXIT_FAILURE;
		else
			status = execute_builtin(shell, type, cmd);
		ft_free2d((void **) cmd->argv);
		destroy_exec_cmd(cmd);
		destroy_shell(shell);
		exit(status);
	}
	return (EXIT_SUCCESS);
}

int	do_builtin_cmd(
		t_shell *shell,
		t_cmd *cmd,
		int fds[2],
		bool async)
{
	t_builtin_type	type;

	type = get_builtin_type(cmd->bin);
	if (type == NA_BUILTIN)
		return (EXIT_BIN_NOT_FOUND);
	if (async)
		return (async_execution(shell, cmd, type, fds));
	return (std_execution(shell, cmd, type, fds));
}
