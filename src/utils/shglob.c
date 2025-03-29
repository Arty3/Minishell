#include <fnmatch.h>

#include "minishell.h"

static bool	shell_glob_match(
				t_list *wildcards,
				const char *pattern,
				const char *path,
				size_t offset);

static
bool	is_wildcard_at(t_list *wildcards, size_t index)
{
	while (wildcards != NULL)
	{
		if (((t_wildcard_pos *) wildcards->content)->index == index)
			return (true);
		wildcards = wildcards->next;
	}
	return (false);
}

static
bool	shell_glob_match_wildcard(
	t_list *wildcards,
	const char *pattern,
	const char *path,
	size_t offset)
{
	size_t	index;
	size_t	pattern_index;

	index = 0;
	pattern_index = 0;
	while (is_wildcard_at(wildcards, pattern_index + offset)
		&& pattern[pattern_index] != '\0')
	{
		wildcards = wildcards->next;
		++pattern_index;
	}
	if (pattern[pattern_index] == '\0' && wildcards == NULL)
		return (true);
	while (path[index] != '\0')
	{
		if (pattern[pattern_index] == path[index]
			&& shell_glob_match(wildcards,
				pattern + pattern_index, path + index, pattern_index + offset))
			return (true);
		++index;
	}
	return (false);
}

static
bool	shell_glob_match(
	t_list *wildcards,
	const char *pattern,
	const char *path,
	size_t offset)
{
	size_t	index;
	size_t	pattern_index;

	index = 0;
	pattern_index = 0;
	while (pattern[pattern_index] != '\0')
	{
		if (pattern[pattern_index] == '*'
			&& is_wildcard_at(wildcards, pattern_index + offset))
			return (shell_glob_match_wildcard(wildcards,
					pattern + pattern_index,
					path + index, pattern_index + offset));
		else if (pattern[pattern_index] != path[index])
			return (false);
		++index;
		++pattern_index;
	}
	return (path[index] == '\0');
}

// A glob function wrapper to accommodate for quoted wildcards.
// E.g. the following example different behavior based on quoted and non-quoted
// wildcards used interchangeably:
//
// ```
// "*"*
// ```
//
// This should use the first wildcard as part of the result name and the second
// to actual perform expansion on. The filename is forced to start with the
// literal `*' character.
t_list	*shell_glob(t_list *wildcards, const char *pattern)
{
	t_list	*paths;
	t_list	*filtered_paths;
	t_list	*path;
	t_list	*temp;

	paths = sglob(pattern, 0);
	path = paths;
	filtered_paths = NULL;
	while (path != NULL)
	{
		if (shell_glob_match(wildcards, pattern, (char *) path->content, 0))
		{
			temp = path->next;
			path->next = NULL;
			ft_append_lst(&filtered_paths, path);
			path = temp;
		}
		else
		{
			temp = path->next;
			ft_pop_lst(path, free);
			path = temp;
		}
	}
	return (filtered_paths);
}
