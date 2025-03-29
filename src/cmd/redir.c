#include "minishell.h"

int	redirect_io(int infd, int outfd)
{
	if (infd != -1)
	{
		if (dup2(infd, STDIN_FILENO) == -1)
			return (-1);
		if (close(infd) == -1)
			return (-1);
	}
	if (outfd != -1)
	{
		if (dup2(outfd, STDOUT_FILENO) == -1)
			return (-1);
		if (close(outfd) == -1)
			return (-1);
	}
	return (0);
}

int	sh_redirect_io(t_shell *shell, int infd, int outfd)
{
	if (redirect_io(infd, outfd) == -1)
		return (-1);
	if (!fd_bitmap_set_fd(&shell->fd, infd, CLOSE_RESET))
		return (-1);
	if (!fd_bitmap_set_fd(&shell->fd, outfd, CLOSE_RESET))
		return (-1);
	return (0);
}
