#include "minishell.h"

static
bool	parse_linebreak_arg(char *argv[], size_t *index)
{
	bool	use_linebreak;
	size_t	i;

	i = 0;
	use_linebreak = true;
	while (argv[i] != NULL)
	{
		if (!ft_strstartswith(argv[i], "-"))
			break ;
		if (*(argv[i] + 1) != 'n')
			break ;
		if (ft_strspn(argv[i] + 1, "n") != ft_strlen(argv[i]) - 1)
			break ;
		use_linebreak = false;
		(*index)++;
		++i;
	}
	return (use_linebreak);
}

int	builtin_echo(int argc, char *argv[], char *envp[])
{
	size_t	i;
	bool	use_linebreak;

	i = 1;
	(void)argc;
	(void)envp;
	use_linebreak = parse_linebreak_arg(argv + 1, &i);
	while (argv[i])
	{
		ft_write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			ft_write(STDOUT_FILENO, " ", 1);
		++i;
	}
	if (use_linebreak)
		ft_write(STDOUT_FILENO, "\n", 1);
	return (EXIT_SUCCESS);
}
