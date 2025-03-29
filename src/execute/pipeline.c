#include "minishell.h"

static
bool	pipeline_pipe_finish(t_shell *shell, int pipe[2], int *in_fd)
{
	if (*in_fd > -1 && ft_close(*in_fd) == -1)
		return (false);
	if (pipe[STDOUT_FILENO] > -1 && ft_close(pipe[STDOUT_FILENO]) == -1)
		return (false);
	*in_fd = pipe[STDIN_FILENO];
	fd_bitmap_close(shell->fd, CLOSE_POST_EXEC_PARENT);
	return (true);
}

static
int	wait_for_pipeline(t_shell *shell, t_pipeline *pipeline, pid_t last_pid)
{
	pid_t	pid;
	t_list	*cmd_node;
	int		stat_loc;
	int		exit_status;
	bool	signaled;

	cmd_node = pipeline->commands;
	exit_status = shell->exit_status;
	stat_loc = -1;
	signaled = false;
	while (cmd_node != NULL)
	{
		pid = wait(&stat_loc);
		if (pid == last_pid && last_pid > -1)
			exit_status = get_exit_status(stat_loc);
		if (!signaled && WIFSIGNALED(stat_loc))
		{
			print_exit_signal(stat_loc);
			signaled = true;
		}
		cmd_node = cmd_node->next;
	}
	return (exit_status);
}

// Swap the READ END with the previous READ END.
static
void	swap_pipes(int swap[2], int fds[3])
{
	swap[0] = fds[0];
	swap[1] = fds[2];
}

static
pid_t	execute_pipeline_cmd(t_shell *shell, t_list *cmd_node,
	int fds[3], bool async)
{
	pid_t	pid;
	int		cmd_fds[2];

	swap_pipes(cmd_fds, fds);
	if (!fd_bitmap_set_fd(&shell->fd, fds[1], CLOSE_PRE_EXEC_CHILD))
		return (-1);
	pid = execute_command(shell, (t_command *) cmd_node->content,
			(int *) cmd_fds, async);
	if (!fd_bitmap_set_fd(&shell->fd, fds[1], CLOSE_RESET))
		return (-1);
	fd_bitmap_set_fd(&shell->fd, fds[0], CLOSE_RESET);
	fd_bitmap_set_fd(&shell->fd, fds[2], CLOSE_RESET);
	return (pid);
}

int	execute_pipeline(t_shell *shell, t_pipeline *pipeline)
{
	t_list	*cmd_node;
	int		pipe[2];
	int		in_fd;
	pid_t	pid;
	size_t	cmd_size;

	cmd_node = pipeline->commands;
	cmd_size = ft_size_lst(pipeline->commands);
	in_fd = -1;
	pid = -1;
	while (cmd_node != NULL)
	{
		ft_memset(&pipe, -1, sizeof(pipe));
		if (cmd_node->next != NULL && ft_pipe(pipe) == -1)
			return (EXIT_FAILURE);
		pid = execute_pipeline_cmd(shell, cmd_node,
				(int []){in_fd, pipe[0], pipe[1]}, cmd_size > 1);
		if (!pipeline_pipe_finish(shell, pipe, &in_fd))
			return (EXIT_FAILURE);
		cmd_node = cmd_node->next;
	}
	return (wait_for_pipeline(shell, pipeline, pid));
}
