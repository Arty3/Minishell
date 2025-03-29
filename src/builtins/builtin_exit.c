#include "minishell.h"

static
bool	handle_exit_arg(const char *str, intmax_t *exit_status)
{
	char	msg[MAX_ERROR_LEN];

	if (!is_valid_number(str, exit_status))
	{
		ft_memset(msg, 0, sizeof(msg));
		ft_strlcat(msg, str, MAX_ERROR_LEN);
		ft_strlcat(msg, ": numeric argument required", MAX_ERROR_LEN);
		print_labeled_error("exit", msg);
		return (false);
	}
	return (true);
}

int	builtin_exit(t_cmd *cmd, t_shell *shell)
{
	intmax_t	exit_status;
	bool		valid_exit_status;

	valid_exit_status = true;
	if (shell->state & STPARENT)
		ft_write(STDERR_FILENO, "exit\n", 5);
	if (cmd->argc == 1)
		exit_status = shell->exit_status;
	else
		valid_exit_status = handle_exit_arg(cmd->argv[1], &exit_status);
	if (valid_exit_status && cmd->argc > 2)
	{
		print_labeled_error("exit", "too many arguments");
		return (EXIT_FAILURE);
	}
	if (!valid_exit_status)
		exit_status = EXIT_INVALID_USAGE;
	exit_status %= 256;
	ft_free2d((void **) cmd->argv);
	destroy_exec_cmd(cmd);
	destroy_shell(shell);
	exit(exit_status);
}
