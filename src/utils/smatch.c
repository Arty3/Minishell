#include <fnmatch.h>
#include <stddef.h>

#include "minishell.h"

static
int	smatch_wildcard(const char *pattern, const char *string)
{
	size_t	index;
	size_t	s_index;

	index = 0;
	s_index = 0;
	while (pattern[index] == '*' && pattern[index] != '\0')
		++index;
	if (pattern[index] == '\0')
		return (0);
	while (string[s_index] != '\0')
	{
		if (pattern[index] == string[s_index]
			&& smatch(pattern + index, string + s_index) == 0)
			return (0);
		++s_index;
	}
	return (FNM_NOMATCH);
}

int	smatch(const char *pattern, const char *string)
{
	size_t	index;
	size_t	s_index;

	index = 0;
	s_index = 0;
	while (pattern[index] != '\0')
	{
		if (pattern[index] == '*')
			return (smatch_wildcard(pattern + index, string + s_index));
		else if (pattern[index] != string[s_index])
			return (FNM_NOMATCH);
		++index;
		++s_index;
	}
	if (string[index] == '\0')
		return (0);
	return (FNM_NOMATCH);
}
