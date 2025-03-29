#include "minishell.h"

bool	execute_and_or_list(t_shell *shell, t_list *and_or_list)
{
	t_list		*node;
	t_and_or	*and_or;
	int32_t		status;

	node = and_or_list;
	status = EXIT_SUCCESS;
	while (node != NULL)
	{
		and_or = node->content;
		if (and_or->type == 0)
			status = execute_pipeline(shell, and_or->pipeline);
		else if (status == EXIT_SUCCESS && and_or->type == T_AND_IF)
			status = execute_pipeline(shell, and_or->pipeline);
		else if (status != EXIT_SUCCESS && and_or->type == T_OR_IF)
			status = execute_pipeline(shell, and_or->pipeline);
		shell->exit_status = status;
		node = node->next;
	}
	return (true);
}
