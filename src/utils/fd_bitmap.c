#include "minishell.h"

t_fd_bitmap	*fd_bitmap_create(size_t size)
{
	t_fd_bitmap	*fdbm;

	fdbm = ft_calloc(1, sizeof(t_fd_bitmap));
	if (fdbm == NULL)
		return (NULL);
	if (size != 0)
		fdbm->bitmap = ft_calloc(1, size);
	else
		fdbm->bitmap = NULL;
	fdbm->size = size;
	return (fdbm);
}

bool	fd_bitmap_set_fd(t_fd_bitmap **fdbm, int fd, int level)
{
	t_fd_bitmap	*new_fdbm;

	if (fdbm == NULL)
		return (false);
	if (*fdbm == NULL)
		*fdbm = fd_bitmap_create(fd + 8);
	if (*fdbm == NULL)
		return (false);
	if (fd < 0)
		return (true);
	if ((size_t) fd > (*fdbm)->size)
	{
		new_fdbm = fd_bitmap_create(fd + 8);
		if (new_fdbm == NULL)
			return (false);
		ft_memcpy(new_fdbm->bitmap, (*fdbm)->bitmap, (*fdbm)->size);
		fd_bitmap_destroy(*fdbm);
	}
	else
		new_fdbm = *fdbm;
	new_fdbm->bitmap[fd] = level;
	return (true);
}

void	fd_bitmap_close(t_fd_bitmap *fdbm, int level)
{
	size_t	i;

	i = 0;
	if (fdbm == NULL)
		return ;
	while (i < fdbm->size)
	{
		if ((level == -1 && fdbm->bitmap[i] != 0)
			|| fdbm->bitmap[i] == level)
		{
			close(i);
			fdbm->bitmap[i] = 0;
		}
		++i;
	}
}

void	fd_bitmap_destroy(t_fd_bitmap *fdbm)
{
	if (fdbm == NULL)
		return ;
	if (fdbm->bitmap != NULL)
		free(fdbm->bitmap);
	free(fdbm);
}
