#include "../minishell.h"

int	check_pipe_scnd(void)
{
	int	i;

	i = 0;
	while (g_global.input[i])
		i++;
	if (i > 0)
		i--;
	while (g_global.input[i] == ' ' && i > 0)
		i--;
	return (i);
}

int	check_pipe(void)
{
	int	i;

	if (g_global.input == NULL)
		return (1);
	i = check_pipe_scnd();
	if (g_global.input[i] == '|')
	{
		if (i > 0)
			i--;
		while (g_global.input[i] == ' ' && i > 0)
			i--;
		if (g_global.input[i] == '|')
			return (1);
	}
	g_global.input = close_pipe(g_global.input);
	if (!g_global.input)
	{
		printf("dashBash: syntax error: unexpected end of file\n");
		return (0);
	}
	return (1);
}

void	sig_int(int sig)
{
	(void)sig;
	if (g_global.f)
	{
		signal(SIGINT, SIG_IGN);
		write(1, "\n", 1);
	}
	if (!g_global.f)
	{
		rl_on_new_line();
		rl_redisplay();
		write(1, "  \b\b\n", 5);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
 	}
}
