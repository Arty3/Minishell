#include "minishell.h"

size_t	get_array_size(char *arr[])
{
	size_t	i;

	i = 0;
	while (arr != NULL && arr[i] != NULL)
		++i;
	return (i);
}

char	**clone_str_array(char *arr[])
{
	char	**clone;
	size_t	i;
	size_t	str_len;

	i = 0;
	if (arr == NULL)
		return (NULL);
	clone = ft_calloc(get_array_size(arr) + 1, sizeof(char *));
	if (clone == NULL)
		return (NULL);
	while (arr[i] != NULL)
	{
		str_len = ft_strlen(arr[i]);
		clone[i] = ft_calloc(str_len + 1, sizeof(char));
		if (clone[i] == NULL)
			return (ft_free2d((void **) clone), NULL);
		clone[i] = ft_memcpy(clone[i], arr[i], str_len);
		i++;
	}
	return (clone);
}

char	**sort_str_array(char *env[],
	int (*cmp)(const char *p1, const char *p2))
{
	size_t	size;
	size_t	i;
	bool	is_sorted;
	char	*tmp;

	size = get_array_size(env);
	is_sorted = false;
	while (env != NULL && size > 0 && !is_sorted)
	{
		i = 0;
		is_sorted = true;
		while (i < size - 1)
		{
			if (cmp(env[i], env[i + 1]) > 0)
			{
				tmp = env[i];
				env[i] = env[i + 1];
				env[i + 1] = tmp;
				is_sorted = false;
			}
			++i;
		}
		--size;
	}
	return (env);
}

char	**create_sorted_array(char *arr[],
	int (*cmp)(const char *ptr1, const char *ptr2))
{
	char	**clone;

	clone = clone_str_array(arr);
	if (clone == NULL)
		return (NULL);
	return (sort_str_array(clone, cmp));
}
