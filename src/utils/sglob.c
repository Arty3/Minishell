#include <fnmatch.h>

#include "minishell.h"

static
t_list	*sglob_wildcard_path_2(
	const char *pattern,
	t_glob *glob,
	const char *absolute_path,
	const char *new_pattern)
{
	char	*sub_path;
	char	*path_value;
	t_list	*paths;

	paths = NULL;
	if (glob->segment[0] != '.' && glob->dirent->d_name[0] == '.')
		return (NULL);
	if (smatch(glob->segment, glob->dirent->d_name) == FNM_NOMATCH)
		return (NULL);
	sub_path = path_join(absolute_path, glob->dirent->d_name, false);
	if (sub_path == NULL)
		return (NULL);
	if (!is_glob_path_valid(sub_path, glob))
		return (free(sub_path), NULL);
	if (glob->has_next_segment)
		paths = sglob(new_pattern, glob->next_segment
				- pattern + (ft_strlen(new_pattern) - ft_strlen(pattern)));
	else if (smatch(pattern, new_pattern) != FNM_NOMATCH)
	{
		path_value = ft_strdup(new_pattern);
		if (path_value == NULL)
			return (free(sub_path), NULL);
		paths = ft_init_lst(path_value);
	}
	return (free(sub_path), paths);
}

static
t_list	*sglob_wildcard_path(
	const char *p,
	t_glob *gl,
	const char *absolute_path)
{
	DIR		*dir;
	char	*new_pattern;
	t_list	*new_paths;
	t_list	*paths;

	paths = NULL;
	if (absolute_path == NULL)
		return (NULL);
	dir = opendir(absolute_path);
	if (dir == NULL)
		return (NULL);
	gl->dirent = readdir(dir);
	while (gl->dirent != NULL)
	{
		new_pattern = path_replace_segment(p, gl->dirent->d_name,
				gl->segment_index);
		if (new_pattern == NULL)
			return (ft_clear_lst(&paths, free), closedir(dir), NULL);
		new_paths = sglob_wildcard_path_2(p, gl, absolute_path, new_pattern);
		if (new_paths != NULL)
			ft_append_lst(&paths, new_paths);
		free(new_pattern);
		gl->dirent = readdir(dir);
	}
	return (closedir(dir), paths);
}

// Segments (or in this case the whole pattern) are considered completed
// whenever there are no wildcard characters (asterisks) remaining.
static
t_list	*sglob_completed_segment(const char *pattern, t_glob *glob)
{
	char	*new_pattern;

	if (glob->has_next_segment)
		return (NULL);
	if (!is_glob_path_valid(pattern, glob))
		return (NULL);
	new_pattern = ft_strdup(pattern);
	if (new_pattern == NULL)
		return (NULL);
	return (ft_init_lst(new_pattern));
}

static
t_list	*sglob_segment(const char *pattern, t_glob *gl)
{
	char	*absolute_path;
	t_list	*paths;

	paths = NULL;
	gl->base_path = get_base_path(pattern);
	if (gl->base_path == NULL)
		return (NULL);
	gl->segment = ft_substr(pattern, gl->segment_index, gl->segment_length);
	if (gl->segment == NULL)
		return (NULL);
	gl->next_segment = ft_strchr(pattern + gl->segment_index, '/');
	gl->has_next_segment = gl->next_segment != NULL
		&& gl->next_segment[1] != '\0';
	if (ft_strchr(gl->segment, '*') != NULL)
	{
		absolute_path = path_join(gl->base_path,
				ft_substr(pattern, 0, gl->segment_index), true);
		paths = sglob_wildcard_path(pattern, gl, absolute_path);
		free(absolute_path);
	}
	else
		paths = sglob_completed_segment(pattern, gl);
	free(gl->segment);
	gl->segment = NULL;
	return (free(gl->base_path), paths);
}

t_list	*sglob(const char *pattern, size_t offset)
{
	t_glob	glob;
	t_list	*paths;
	t_list	*new_paths;
	size_t	previous_segment_index;

	paths = NULL;
	new_paths = NULL;
	previous_segment_index = offset;
	ft_memset(&glob, 0, sizeof(t_glob));
	while (true)
	{
		glob.segment_length = ft_strcspn(pattern
				+ previous_segment_index, "/");
		glob.segment_index = previous_segment_index;
		if (glob.segment_length != 0)
			new_paths = sglob_segment(pattern, &glob);
		if (new_paths != NULL)
			ft_append_lst(&paths, new_paths);
		if (pattern[glob.segment_index + glob.segment_length] == '\0')
			break ;
		new_paths = NULL;
		previous_segment_index = glob.segment_index + glob.segment_length + 1;
	}
	return (paths);
}
