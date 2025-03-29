#include "minishell.h"

#include <termios.h>

int	g_sigstat = 0;

static
int	assign_sig_trap(int32_t __signo, void (sig_handler)(int))
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sig_handler;
	if (sigemptyset(&sa.sa_mask) == -1)
		return (-1);
	if (sigaction(__signo, &sa, NULL) == -1)
		return (-1);
	return (0);
}

static
void	sigint_prompt_trap(int32_t __signo)
{
	g_sigstat = __signo;
	ft_write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static
void	sigint_heredoc_trap(int32_t __signo)
{
	struct termios	old_termios;
	struct termios	temp_termios;

	g_sigstat = __signo;
	tcgetattr(STDIN_FILENO, &old_termios);
	tcgetattr(STDIN_FILENO, &temp_termios);
	temp_termios.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &temp_termios);
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

bool	assign_shell_traps(int type)
{
	__sighandler_t	sigint_handler;
	__sighandler_t	sigquit_handler;

	sigint_handler = SIG_DFL;
	sigquit_handler = SIG_DFL;
	if (type == SIG_TRAP_SHELL)
		sigint_handler = SIG_IGN;
	if (type == SIG_TRAP_PROMPT)
		sigint_handler = sigint_prompt_trap;
	if (type == SIG_TRAP_HEREDOC)
		sigint_handler = sigint_heredoc_trap;
	if (type != SIG_TRAP_RESET)
		sigquit_handler = SIG_IGN;
	if (assign_sig_trap(SIGINT, sigint_handler) == -1)
		return (false);
	if (assign_sig_trap(SIGQUIT, sigquit_handler) == -1)
		return (false);
	return (true);
}
