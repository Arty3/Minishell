#ifndef CMD_H

# define CMD_H

# include "libft.h"

# include "common.h"

int		redirect_io(int infd, int outfd);
int		sh_redirect_io(t_shell *shell, int infd, int outfd);

int		handle_execve_error(t_cmd *cmd, int __errno);

int		get_exit_status(int stat_loc);
void	print_exit_signal(int signal);
int		wait_for_child(pid_t child);
int		execute_cmd(t_shell *shell, t_cmd *cmd, int fds[2], bool async);

#endif
