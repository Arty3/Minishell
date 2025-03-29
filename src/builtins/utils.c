#include "minishell.h"

bool	is_builtin(const char *__restrict__ literal)
{
	return (get_builtin_type(literal) != NA_BUILTIN);
}

t_builtin_type	get_builtin_type(const char *__restrict__ literal)
{
	if (ft_strcmp(literal, "cd") == 0)
		return (CD);
	if (ft_strcmp(literal, "pwd") == 0)
		return (PWD);
	if (ft_strcmp(literal, "env") == 0)
		return (ENV);
	if (ft_strcmp(literal, "echo") == 0)
		return (ECHO);
	if (ft_strcmp(literal, "exit") == 0)
		return (EXIT);
	if (ft_strcmp(literal, "unset") == 0)
		return (UNSET);
	if (ft_strcmp(literal, "export") == 0)
		return (EXPORT);
	return (NA_BUILTIN);
}
