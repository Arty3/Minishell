#ifndef EXECUTE_H

# define EXECUTE_H

# include "libft.h"
# include "parser.h"

int		execute_pipeline(t_shell *shell, t_pipeline *pipeline);
bool	execute_and_or_list(t_shell *shell, t_list *and_or_list);
pid_t	execute_command(t_shell *shell,
			t_command *command, int pipe_fds[2], bool async);
pid_t	execute_executable_cmd(t_shell *shell, char *argv[], int pipe_fds[2],
			bool async);
int		execute_apply_redirs(t_shell *shell, t_list *redirs, int fds[2]);

int		traverse_heredoc_node(t_shell *shell, t_node *node);
int		traverse_heredoc_node_list(t_shell *shell, t_list *node_list);

int		resolve_binary(const char *bin, char **out, const char *envp[]);
t_cmd	*init_exec_cmd(char *bin, char *argv[], char *envp[]);
void	destroy_exec_cmd(t_cmd *exec_cmd);
#endif
