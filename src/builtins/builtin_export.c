#include "minishell.h"

static
void	print_invalid_identifier(const char *identifier)
{
	char	msg[MAX_ERROR_LEN];

	ft_memset(msg, 0, sizeof(msg));
	ft_strlcat(msg, "`", MAX_ERROR_LEN);
	ft_strlcat(msg, identifier, MAX_ERROR_LEN);
	ft_strlcat(msg, "': not a valid identifier", MAX_ERROR_LEN);
	print_labeled_error("export", msg);
}

static
bool	add_to_envp(char **envp[], char *kv[2], bool assign, bool append)
{
	const char	*prev_value;
	char		*new_value;
	bool		result;
	bool		alloc;

	new_value = (char *) kv[1];
	prev_value = sh_env_get_value(*envp, kv[0]);
	alloc = false;
	if (kv[1] == NULL && prev_value != NULL)
		return (true);
	if (append && prev_value != NULL)
	{
		new_value = ft_strjoin(prev_value, kv[1]);
		alloc = true;
	}
	if (kv[1] == NULL && assign)
		new_value = "";
	result = sh_env_set_value(envp, kv[0], new_value);
	if (alloc)
		free(new_value);
	return (result);
}

static
int	export_arg(char **envp[], const char *arg)
{
	char	*assign;
	bool	append;
	size_t	key_len;
	char	*key;
	char	*value;

	assign = ft_strchr(arg, '=');
	append = assign != NULL && arg != assign && *(assign - 1) == '+';
	if (assign != NULL)
		key_len = (assign - (int) append) - arg;
	else
		key_len = ft_strlen(arg);
	key = ft_substr(arg, 0, key_len);
	if (key == NULL)
		return (EXIT_FAILURE);
	if (!is_valid_identifier(key))
		return (print_invalid_identifier(arg), free(key), EXIT_FAILURE);
	value = NULL;
	if (assign != NULL)
		value = ft_substr(arg, assign - arg + 1, ft_strlen(assign + 1));
	if (!add_to_envp(envp, (char *[2]){key, value}, assign != NULL, append))
		return (free(key), free(value), EXIT_FAILURE);
	return (free(key), free(value), EXIT_SUCCESS);
}

int	builtin_export(int argc, char *argv[], t_shell *shell)
{
	int		status;
	size_t	i;

	i = 1;
	status = EXIT_SUCCESS;
	if (argc == 1)
	{
		if (!sh_env_display_sorted((const char **) shell->envp,
				ENV_PRINT_EXPORT))
		{
			print_labeled_error("env", "failed to display"
				" environment variables");
			return (EXIT_FAILURE);
		}
		return (EXIT_SUCCESS);
	}
	while (argv[i] != NULL)
	{
		if (export_arg(&shell->envp, argv[i]) != EXIT_SUCCESS)
			status = EXIT_FAILURE;
		++i;
	}
	return (status);
}
