#include "minishell.h"

bool	is_valid_number(const char *str, intmax_t *result)
{
	intmax_t	value;
	char		*endptr;

	if (str == NULL)
		return (false);
	if (result != NULL)
		*result = 0;
	errno = 0;
	value = ft_strtoimax(str, &endptr, 10);
	if (errno || endptr == str)
		return (false);
	if (*str != '\0' && *endptr == '\0')
	{
		if (result != NULL)
			*result = value;
		return (true);
	}
	return (false);
}
