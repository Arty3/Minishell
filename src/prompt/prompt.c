#include "minishell.h"

#define FALLBACK_USERNAME "I have no name!"

static
void	prompt_put_home(char *buf, const char *pwd, char *envp[])
{
	const char	*home = sh_env_get_value(envp, "HOME");
	size_t		home_len;

	if (home == NULL || !ft_strstartswith(pwd, home))
	{
		ft_strlcat(buf, pwd, PATH_MAX);
		return ;
	}
	home_len = ft_strlen(home);
	ft_strlcat(buf, "~", PATH_MAX);
	ft_strlcat(buf, pwd + home_len, PATH_MAX);
}

static
bool	populate_prompt_buf(char *buf, char *envp[])
{
	const char	*pwd = sh_env_get_value(envp, "PWD");
	char		cwd[PATH_MAX];
	const char	*ret = ft_getcwd(cwd, PATH_MAX);
	const char	*usr = sh_env_get_value(envp, "USER");

	ft_memset(buf, 0, PATH_MAX);
	ft_strlcat(buf, "\001" COLOR_GREEN "\002", PATH_MAX);
	if (usr)
		ft_strlcat(buf, usr, PATH_MAX);
	else
		ft_strlcat(buf, FALLBACK_USERNAME, PATH_MAX);
	ft_strlcat(buf, "\001" COLOR_RESET "\002", PATH_MAX);
	if (pwd || ret)
	{
		ft_strlcat(buf, ":", PATH_MAX);
		ft_strlcat(buf, "\001" COLOR_BLUE "\002", PATH_MAX);
	}
	if (pwd)
		prompt_put_home(buf, pwd, envp);
	else if (ret)
		prompt_put_home(buf, cwd, envp);
	ft_strlcat(buf, "\001" COLOR_RESET "\002", PATH_MAX);
	ft_strlcat(buf, "$ ", PATH_MAX);
	return (true);
}

static
void	handle_lexer_error(t_shell *shell, t_lexer *lexer)
{
	if (lexer->error == NULL)
		return ;
	print_labeled_error("lexer error", lexer->error);
	shell->exit_status = EXIT_PARSER_ERROR;
}

static
bool	handle_line(t_shell *shell, const char *line)
{
	shell->lexer = lexer_create(line);
	if (shell->lexer == NULL)
		return (false);
	if (!lexer_tokenize(shell->lexer))
	{
		handle_lexer_error(shell, shell->lexer);
		if (shell->lexer->error == NULL)
		{
			lexer_destroy(shell->lexer);
			shell->lexer = NULL;
			return (false);
		}
		lexer_destroy(shell->lexer);
		shell->lexer = NULL;
		return (true);
	}
	if (g_sigstat == SIGINT)
		shell->exit_status = EXIT_SIGINT;
	if (!execute_from_lexer(shell, shell->lexer))
		return (false);
	lexer_destroy(shell->lexer);
	shell->lexer = NULL;
	return (true);
}

bool	prompt(t_shell *shell)
{
	char	prompt[PATH_MAX + USR_MAX];

	g_sigstat = 0;
	if (!populate_prompt_buf(prompt, shell->envp))
		return (false);
	if (!assign_shell_traps(SIG_TRAP_PROMPT))
		return (print_error(ENOSIG), false);
	shell->current_line = readline(prompt);
	if (!assign_shell_traps(SIG_TRAP_SHELL))
		return (print_error(ENOSIG), false);
	if (shell->current_line == NULL)
	{
		ft_write(STDERR_FILENO, "exit\n", 5);
		return (false);
	}
	if (!handle_line(shell, shell->current_line))
	{
		print_labeled_error("error", "failed to handle line");
		return (false);
	}
	if (*shell->current_line != '\0')
		add_history(shell->current_line);
	free(shell->current_line);
	shell->current_line = NULL;
	return (true);
}
