#include "minishell.h"

bool	is_valid_identifier(const char *identifier)
{
	size_t	i;

	i = 0;
	if (identifier[0] == '\0')
		return (false);
	if (ft_isdigit(identifier[0]))
		return (false);
	while (identifier[i] != '\0')
	{
		if (!ft_isalnum(identifier[i]) && identifier[i] != '_')
			return (false);
		++i;
	}
	return (true);
}
