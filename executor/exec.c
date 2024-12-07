/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:08:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/06 18:18:43 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	monitor(t_minishell *t_m, size_t i)
{
	if (i > 0)
	{
		close(t_m->pipes_fd[i - 1][0]);
		close(t_m->pipes_fd[i - 1][1]);
	}
	if (i == t_m->cmd_count - 1)
	{
		close(t_m->pipes_fd[i][0]);
		close(t_m->pipes_fd[i][1]);
	}
	return (0);
}

static int	ft_soloexec(t_minishell *t_m, size_t i, int c_int)
{
	t_cmd	*c;
	char	**nenv;

	c = &(t_m->cmds[i]);
	nenv = pipe_env(t_m);
	if (i < t_m->cmd_count)
		if (pipe(t_m->pipes_fd[i]) == -1)
			return (-1);
	signalignore(SIGINT);
	t_m->pid[i] = fork();
	if (t_m->pid[i] == -1)
		return (-1);
	if (!t_m->pid[i])
		child_molestor(t_m, c, i, c_int, nenv);
	else
		monitor(t_m, i);
	free(nenv);
	return (1);
}

static int	ft_waiter(t_minishell *t_m)
{
	size_t	i;

	i = 0;
	t_m->exstat = 0;
	while (i < (t_m->cmd_count))
	{
		waitpid(t_m->pid[i], &t_m->exstat, 0);
		t_m->exstat = WEXITSTATUS(t_m->exstat);
		/*if (WIFSIGNALED(t_m->exstat))
		{
			t_m->exstat = 128 + WTERMSIG(t_m->exstat);
			write(2, "\n", 1);
		}*/
		i++;
	}
	return (0);
}

static int	executions(t_minishell *t_m, size_t i)
{
	t_cmd	*c;
	int		c_int;

	c = &(t_m->cmds[i]);
	if (c->command == NULL || !ft_strlen(c->command)
		|| !ft_strcmp(c->command, " "))
		ft_soloexec(t_m, i, -1);
	else
	{
		if (!c->command || !FT(c->command))
			c_int = -1;
		else
		{
			t_m->c_args = ft_split_quotes(c->command, ' ', 0);
			c_int = is_builtin(ft_rm_quotes(t_m->c_args[0]));
		}
		if (c_int != -1 && t_m->cmd_count == 1)
			return (builtindirector(t_m, c, c_int), FTF(t_m->c_args), -1);
		else
			return (ft_soloexec(t_m, i, c_int), FTF(t_m->c_args), EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}

void	exec_cmds(t_minishell *t_m)
{
	size_t	i;
	int		a;

	i = 0;
	a = 0;
	free(t_m->pid);
	free(t_m->pipes_fd);
	t_m->pid = ft_calloc(t_m->cmd_count + 1, sizeof(pid_t));
	t_m->pipes_fd = ft_calloc(t_m->cmd_count + 1, sizeof(int [2]));
	if (!t_m->pipes_fd || !t_m->pid)
		return (perror("arrays creation error."), exit(1));
	if (heredocalloc(t_m) != -1)
		while (i < t_m->cmd_count)
			a += executions(t_m, i++);
	else
		write(2, "\n", 1);
	if (a >= 0)
		ft_waiter(t_m);
	delete_heredocs(t_m);
	signalsetter(SIGINT, handler);
}
