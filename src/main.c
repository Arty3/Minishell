#include "minishell.h"

static
t_shell	*init_shell(void)
{
	t_shell	*shell;

	shell = ft_calloc(1, sizeof(t_shell));
	if (shell == NULL)
		return (NULL);
	shell->fd = fd_bitmap_create(DEFAULT_BITMAP_SIZE);
	if (shell->fd == NULL)
	{
		destroy_shell(shell);
		return (NULL);
	}
	shell->state = STPARENT;
	return (shell);
}

void	destroy_shell(t_shell *shell)
{
	if (shell == NULL)
		return ;
	if (shell->current_line != NULL)
		free(shell->current_line);
	if (shell->lexer != NULL)
		lexer_destroy(shell->lexer);
	if (shell->parser != NULL)
		parser_destroy(shell->parser);
	if (shell->program != NULL)
		parser_destroy_program(shell->program);
	if (shell->envp != NULL)
		ft_free2d((void **) shell->envp);
	if (shell->fd != NULL)
	{
		fd_bitmap_close(shell->fd, CLOSE_ALL);
		fd_bitmap_destroy(shell->fd);
	}
	rl_clear_history();
	free(shell);
}

static
bool	env_init(t_shell *shell, char *envp[])
{
	char	cwd[PATH_MAX];

	shell->envp = clone_str_array(envp);
	if (shell->envp == NULL)
		return (false);
	shell->default_home = (char *) sh_env_get_value(envp, "HOME");
	if (!ft_getcwd(cwd, PATH_MAX))
		return (false);
	if (!sh_env_set_value(&shell->envp, "PWD", cwd))
		return (false);
	if (!sh_env_set_value(&shell->envp, "OLDPWD", cwd))
		return (false);
	if (!env_adjust_shlvl(&shell->envp))
		return (false);
	return (true);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	*shell;
	int		last_exit_status;

	(void)argc;
	(void)argv;
	shell = init_shell();
	if (shell == NULL || !env_init(shell, envp))
	{
		print_labeled_error("error", "failed to initialize shell");
		destroy_shell(shell);
		return (EXIT_FAILURE);
	}
	while (prompt(shell))
		;
	last_exit_status = shell->exit_status;
	destroy_shell(shell);
	return (last_exit_status);
}
