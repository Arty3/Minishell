#ifndef UTILS_H

# define UTILS_H

# include "libft.h"
# include "common.h"

# include <sys/stat.h>
# include <dirent.h>
# include <pwd.h>

# define MAX_ERROR_LEN 8192

enum e_error_type
{
	ENOFORK, /* failed to fork */
	ENOSIG, /* failed to intercept signal */
	EMAX
};

__inline__ bool	is_directory(const char *__restrict__ path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (false);
	return (S_ISDIR(statbuf.st_mode));
}

__inline__ bool	is_path_relative(const char *__restrict__ path)
{
	return (path != NULL && *path != '/');
}

__inline__ bool	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

__inline__ bool	is_executable(const char *__restrict__ path)
{
	return (ft_access(path, X_OK) == 0);
}

bool			is_builtin(const char *__restrict__ literal);
t_builtin_type	get_builtin_type(const char *__restrict__ literal);

char			*get_base_path(const char *pattern);
bool			is_glob_path_valid(const char *path, t_glob *glob);
char			*path_join(const char *path, char *sub, bool free_sub);
char			*path_replace_segment(
					const char *src,
					const char *replace_with,
					size_t at);
char			*heredoc_join_line(t_redir *redir, char *line);
void			print_error(enum e_error_type type);
void			print_labeled_error(const char *label, const char *error_msg);
t_list			*sglob(const char *pattern, size_t offset);
t_list			*shell_glob(t_list *wildcards, const char *pattern);
int				smatch(const char *pattern, const char *string);
bool			is_valid_identifier(const char *identifier);
bool			is_valid_number(const char *str, intmax_t *result);

size_t			get_array_size(char *arr[]);
char			**clone_str_array(char *arr[]);
char			**sort_str_array(char *env[],
					int (*cmp)(const char *p1, const char *p2));
char			**create_sorted_array(char *arr[],
					int (*cmp)(const char *ptr1, const char *ptr2));

t_fd_bitmap		*fd_bitmap_create(size_t size);
bool			fd_bitmap_set_fd(t_fd_bitmap **fdbm, int fd, int level);
void			fd_bitmap_close(t_fd_bitmap *fdbm, int level);
void			fd_bitmap_destroy(t_fd_bitmap *fdbm);

#endif
