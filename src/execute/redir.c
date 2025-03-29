#include "minishell.h"

static
int	__open_file(int *fd, const char *literal, int flag, int permcode)
{
	if (*fd > -1 && ft_close(*fd) == -1)
		return (-1);
	if (permcode != -1)
		*fd = open(literal, flag, permcode);
	else
		*fd = open(literal, flag);
	if (*fd == -1)
	{
		print_labeled_error(literal, strerror(errno));
		return (-1);
	}
	return (0);
}

static
bool	execute_apply_heredoc(t_shell *shell, t_redir *redir,
	char *eof, int fds[2])
{
	char	*content;
	int		heredoc_fds[2];

	if (fds[0] > -1 && ft_close(fds[0]) == -1)
		return (free(eof), EXIT_FAILURE);
	if (ft_pipe(heredoc_fds) == -1)
		return (free(eof), EXIT_FAILURE);
	fds[0] = heredoc_fds[0];
	content = expander_expand_heredoc(redir, shell);
	if (content == NULL)
		content = ft_strdup(redir->value);
	if (content == NULL)
		return (free(eof), EXIT_FAILURE);
	ft_write(heredoc_fds[1], content, ft_strlen(content));
	ft_close(heredoc_fds[1]);
	free(content);
	free(eof);
	return (EXIT_SUCCESS);
}

// 0 = fd_idx
// 1 = flags
// 2 = permcode
// redir->type % 3 = [0,1,2]
static
bool	assign_redir_conf(t_redir *redir, int conf[3])
{
	const int	redir_map[T_WORD][3] = {[T_LESS] = {0, O_RDONLY, -1},
	[T_GREAT] = {1, O_WRONLY | O_CREAT | O_TRUNC, 0644},
	[T_DGREAT] = {1, O_WRONLY | O_CREAT | O_APPEND, 0644}};

	if (redir->type != T_LESS && redir->type != T_GREAT
		&& redir->type != T_DGREAT)
		return (false);
	ft_memcpy(conf, redir_map[redir->type], sizeof(int [3]));
	return (true);
}

static
bool	execute_apply_redir(t_shell *shell, t_redir *redir, int fds[2])
{
	char	*name;
	int		conf[3];

	name = expander_expand_redir_name(redir, (const char **) shell->envp);
	if (name == NULL)
	{
		print_labeled_error(redir->name->raw, "ambiguous redirect");
		return (EXIT_FAILURE);
	}
	ft_memset(&conf, 0, sizeof(conf));
	if (assign_redir_conf(redir, conf))
	{
		if (__open_file(&fds[conf[0]], name, conf[1], conf[2]) == -1)
			return (free(name), EXIT_FAILURE);
	}
	else
		return (execute_apply_heredoc(shell, redir, name, fds));
	return (free(name), EXIT_SUCCESS);
}

int	execute_apply_redirs(t_shell *shell, t_list *redirs, int fds[2])
{
	t_list	*redir_node;
	t_redir	*redir;
	int		new_fds[2];
	int		status;

	redir_node = redirs;
	status = EXIT_SUCCESS;
	ft_memset(&new_fds, -1, sizeof(new_fds));
	while (redir_node != NULL)
	{
		redir = (t_redir *) redir_node->content;
		status = execute_apply_redir(shell, redir, new_fds);
		if (status != EXIT_SUCCESS)
			break ;
		redir_node = redir_node->next;
	}
	fd_bitmap_set_fd(&shell->fd, fds[0], CLOSE_IN_CHILD);
	fd_bitmap_set_fd(&shell->fd, fds[1], CLOSE_IN_CHILD);
	if (new_fds[0] > -1
		&& fd_bitmap_set_fd(&shell->fd, new_fds[0], CLOSE_POST_EXEC_PARENT))
		fds[0] = new_fds[0];
	if (new_fds[1] > -1
		&& fd_bitmap_set_fd(&shell->fd, new_fds[1], CLOSE_POST_EXEC_PARENT))
		fds[1] = new_fds[1];
	return (status);
}
