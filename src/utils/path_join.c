#include "minishell.h"

static
char	*ft_strcpy(char *dest, const char *src)
{
	size_t	src_len;

	src_len = ft_strlen(src);
	ft_memcpy(dest, src, src_len);
	dest[src_len] = '\0';
	return (dest);
}

static
char	*ft_strncpy2(char *dest, const char *src, size_t n)
{
	ft_memcpy(dest, src, n);
	dest[n] = '\0';
	return (dest);
}

char	*path_join(const char *path, char *sub, bool free_sub)
{
	char	*new_path;
	size_t	path_len;

	if (path == NULL || sub == NULL)
		return (NULL);
	path_len = ft_strlen(path);
	new_path = ft_calloc(path_len + ft_strlen(sub) + 2, sizeof(char));
	if (new_path == NULL)
		return (free(sub), NULL);
	ft_strcpy(new_path, path);
	if (path_len > 0 && path[path_len - 1] != '/')
		new_path[path_len++] = '/';
	ft_strcpy(new_path + path_len, sub);
	if (free_sub)
		free(sub);
	return (new_path);
}

char	*path_replace_segment(
	const char *src,
	const char *replace_with,
	size_t at)
{
	const size_t	replace_with_len = strlen(replace_with);
	const size_t	len = strlen(src) + replace_with_len;
	const size_t	offset = at + (src[at] == '/');
	char			*str;

	str = calloc(len + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	ft_strncpy2(str, src, offset);
	ft_strcpy(str + offset, replace_with);
	ft_strcpy(str + offset + replace_with_len,
		src + at + 1 + strcspn(src + at + 1, "/"));
	return (str);
}

char	*heredoc_join_line(t_redir *redir, char *line)
{
	char	*tmp;

	tmp = redir->value;
	redir->value = ft_strjoin(redir->value, line);
	free(tmp);
	free(line);
	if (redir->value == NULL)
		return (NULL);
	tmp = redir->value;
	redir->value = ft_strjoin(redir->value, "\n");
	free(tmp);
	return (redir->value);
}
