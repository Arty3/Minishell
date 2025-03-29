#ifndef BUILTINS_H

# define BUILTINS_H

# include "libft.h"

# include "cmd.h"

int	builtin_cd(int argc, char *argv[], t_shell *shell);
int	builtin_echo(int argc, char *argv[], char *envp[]);
int	builtin_env(int argc, char *argv[], char *envp[]);
int	builtin_export(int argc, char *argv[], t_shell *shell);
int	builtin_pwd(char *envp[]);
int	builtin_unset(int argc, char *argv[], t_shell *shell);
int	builtin_exit(t_cmd *cmd, t_shell *shell);

int	do_builtin_cmd(t_shell *shell, t_cmd *cmd, int fds[2], bool async);

#endif
