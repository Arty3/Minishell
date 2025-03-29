#include "minishell.h"

int	handle_execve_error(t_cmd *cmd, int __errno)
{
	int	status;

	if (__errno == ENOEXEC)
	{
		print_labeled_error(cmd->argv[0], strerror(__errno));
		return (EXIT_BIN_CANT_EXEC);
	}
	if (__errno == ENOENT)
		status = EXIT_BIN_NOT_FOUND;
	else
		status = EXIT_BIN_CANT_EXEC;
	if (is_directory(cmd->argv[0]))
		print_labeled_error(cmd->argv[0], strerror(EISDIR));
	else
		print_labeled_error(cmd->argv[0], strerror(__errno));
	return (status);
}
