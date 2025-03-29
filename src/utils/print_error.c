#include "minishell.h"

void	print_error(enum e_error_type type)
{
	char				msg[MAX_ERROR_LEN];
	static const char	*messages[] = {
	[ENOFORK] = "failed to fork",
	[ENOSIG] = "failed to intercept signal"
	};

	if (type < 0 || type >= EMAX)
		return ;
	ft_memset(msg, 0, sizeof(msg));
	ft_strlcat(msg, PROGRAM_NAME ": ", MAX_ERROR_LEN);
	ft_strlcat(msg, messages[type], MAX_ERROR_LEN);
	ft_strlcat(msg, "\n", MAX_ERROR_LEN);
	ft_write(STDERR_FILENO, msg, ft_strlen(msg) * sizeof(char));
}

void	print_labeled_error(const char *label, const char *error_msg)
{
	const size_t	label_len = ft_strlen(label);
	const size_t	err_msg_len = ft_strlen(error_msg);
	char			*msg;

	msg = ft_calloc(label_len + err_msg_len + 512, sizeof(char));
	if (msg == NULL)
		return ;
	ft_strlcat(msg, PROGRAM_NAME ": ", PATH_MAX);
	ft_strlcat(msg, label, PATH_MAX);
	ft_strlcat(msg, ": ", PATH_MAX);
	ft_strlcat(msg, error_msg, PATH_MAX);
	ft_strlcat(msg, "\n", PATH_MAX);
	ft_write(STDERR_FILENO, msg, ft_strlen(msg));
	free(msg);
}
