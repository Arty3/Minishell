#ifndef MINISHELL_H

# define MINISHELL_H

# include "libft.h"

# include <dirent.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>

# include <readline/readline.h>
# include <readline/history.h>
# include <bits/waitflags.h>
# include <linux/limits.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <pwd.h>

# include "common.h"
# include "lexer.h"
# include "parser.h"
# include "expander.h"
# include "utils.h"
# include "cmd.h"
# include "builtins.h"
# include "execute.h"
# include "env.h"

bool	execute_from_lexer(t_shell *shell, t_lexer *lexer);
bool	prompt(t_shell *shell);
bool	assign_shell_traps(int type);
void	destroy_shell(t_shell *shell);

#endif
