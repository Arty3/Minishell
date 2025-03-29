#include "minishell.h"

static
void	heredoc_print_eof(const char *eof)
{
	char	msg[MAX_ERROR_LEN];

	ft_memset(&msg, 0, sizeof(msg));
	ft_strlcat(msg, "here-document delimited by end-of-file", MAX_ERROR_LEN);
	ft_strlcat(msg, " (wanted `", MAX_ERROR_LEN);
	ft_strlcat(msg, eof, MAX_ERROR_LEN);
	ft_strlcat(msg, "')", MAX_ERROR_LEN);
	print_labeled_error("warning", msg);
}

static
bool	heredoc_read_line(const char *eof, t_redir *redir)
{
	char	*line;
	size_t	eof_len;

	g_sigstat = 0;
	eof_len = ft_strlen(eof);
	while (true)
	{
		assign_shell_traps(SIG_TRAP_HEREDOC);
		line = readline(PS2);
		if (g_sigstat == SIGINT)
			return (free(line), false);
		assign_shell_traps(SIG_TRAP_PROMPT);
		if (line == NULL)
			return (heredoc_print_eof(eof), true);
		if (!ft_strncmp(eof, line, eof_len) && line[eof_len] == '\0')
			return (free(line), true);
		if (!heredoc_join_line(redir, line))
			return (false);
	}
	return (true);
}

static
int	execute_heredoc(t_redir *redir)
{
	char	*eof;

	if (redir->type != T_DLESS)
		return (EXIT_SUCCESS);
	eof = expander_expand_redir_name(redir, NULL);
	if (eof == NULL)
	{
		print_labeled_error(redir->name->raw, "ambiguous redirect");
		return (EXIT_FAILURE);
	}
	redir->value = ft_strdup("");
	if (redir->value == NULL)
		return (free(eof), EXIT_FAILURE);
	if (!heredoc_read_line(eof, redir))
	{
		if (g_sigstat == SIGINT)
			return (free(eof), EXIT_SIGINT);
		return (free(eof), EXIT_FAILURE);
	}
	return (free(eof), EXIT_SUCCESS);
}

int	traverse_heredoc_node_list(t_shell *shell, t_list *node_list)
{
	t_list	*node;
	int		status;

	node = node_list;
	while (node != NULL)
	{
		status = traverse_heredoc_node(shell, (t_node *) node->content);
		if (status != EXIT_SUCCESS)
			return (status);
		node = node->next;
	}
	return (EXIT_SUCCESS);
}

int	traverse_heredoc_node(t_shell *shell, t_node *node)
{
	int	status;

	if (node->node_type == T_PROGRAM)
		return (traverse_heredoc_node_list(shell,
				((t_program *) node)->and_or_list));
	if (node->node_type == T_AND_OR)
		return (traverse_heredoc_node(shell,
				((t_node *)((t_and_or *) node)->pipeline)));
	if (node->node_type == T_PIPELINE)
		return (traverse_heredoc_node_list(shell,
				((t_pipeline *) node)->commands));
	if (node->node_type == T_SIMPLE_COMMAND)
		return (traverse_heredoc_node_list(shell,
				((t_simple_command *) node)->redirs));
	if (node->node_type == T_SUBSHELL)
	{
		status = traverse_heredoc_node_list(shell,
				((t_subshell *) node)->redirs);
		if (status != EXIT_SUCCESS)
			return (status);
		return (traverse_heredoc_node_list(shell,
				((t_subshell *) node)->and_or_list));
	}
	return (execute_heredoc((t_redir *) node));
}
