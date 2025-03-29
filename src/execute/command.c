#include "minishell.h"

static
char	**command_to_argv(t_simple_command *command)
{
	char	**argv;
	t_list	*node;
	size_t	i;

	i = 0;
	if (command->name == NULL)
		return (ft_calloc(1, sizeof(char *)));
	argv = ft_calloc(1 + ft_size_lst(command->args) + 1, sizeof(char *));
	if (argv == NULL)
		return (NULL);
	argv[i++] = ft_strdup(command->name->raw);
	if (argv[i - 1] == NULL)
		return (ft_free2d((void **) argv), NULL);
	node = command->args;
	while (node != NULL)
	{
		argv[i++] = ft_strdup(((t_token *) node->content)->raw);
		if (argv[i - 1] == NULL)
			return (ft_free2d((void **) argv), NULL);
		node = node->next;
	}
	return (argv);
}

static
pid_t	execute_simple_command(t_shell *shell, t_simple_command *command,
	int pipe_fds[2], bool async)
{
	char	**argv;
	int		status;
	pid_t	pid;

	if (!expander_expand_command((t_command *) command, shell))
		return (-1);
	argv = command_to_argv(command);
	if (argv == NULL)
		return (print_labeled_error("error", strerror(ENOMEM)), -1);
	status = execute_apply_redirs(shell, command->redirs, pipe_fds);
	if (status != EXIT_SUCCESS || argv[0] == NULL)
	{
		shell->exit_status = status;
		ft_free2d((void **) argv);
		return (-1);
	}
	if (async)
		shell->state &= ~STPARENT;
	pid = execute_executable_cmd(shell, argv, pipe_fds, async);
	if (async)
		shell->state |= STPARENT;
	ft_free2d((void **) argv);
	return (pid);
}

static
pid_t	execute_subshell(t_shell *shell, t_subshell *subshell, int pipe_fds[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		assign_shell_traps(SIG_TRAP_RESET);
		shell->state &= ~(STPARENT);
		shell->state |= STSUBSHELL;
		status = execute_apply_redirs(shell, subshell->redirs, pipe_fds);
		fd_bitmap_set_fd(&shell->fd, pipe_fds[0], CLOSE_RESET);
		fd_bitmap_close(shell->fd, CLOSE_PRE_EXEC_CHILD);
		if (status != EXIT_SUCCESS)
			shell->exit_status = status;
		else if (sh_redirect_io(shell, pipe_fds[0], pipe_fds[1]) == -1)
			shell->exit_status = EXIT_FAILURE;
		else
			execute_and_or_list(shell, subshell->and_or_list);
		status = shell->exit_status;
		destroy_shell(shell);
		exit(status);
	}
	return (pid);
}

pid_t	execute_command(t_shell *shell, t_command *command,
	int pipe_fds[2], bool async)
{
	if (command->node_type == T_SIMPLE_COMMAND)
		return (execute_simple_command(shell, (t_simple_command *) command,
				pipe_fds, async));
	if (command->node_type == T_SUBSHELL)
		return (execute_subshell(shell, (t_subshell *) command, pipe_fds));
	return (-1);
}
