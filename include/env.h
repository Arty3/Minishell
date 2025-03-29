#ifndef ENV_H
# define ENV_H

# include "libft.h"

void		sh_env_display(const char *envp[], int flags);
bool		sh_env_display_sorted(const char *envp[], int flags);
const char	*sh_env_get_value(char *envp[], const char *name);
bool		sh_env_set_value(char **envp[], const char *name,
				const char *value);
bool		sh_env_append(char **envp[], const char *name, const char *append);
bool		sh_env_unset(char **envp[], const char *name);
bool		env_adjust_shlvl(char **envp[]);

#endif
