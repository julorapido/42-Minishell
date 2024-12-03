/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:08:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/03 08:51:23 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exec2(char *cmd, char **env)
{
	char	**ft_cmd;
	char	*path;

	ft_cmd = ft_split_quotes(cmd, ' ', 0);
	path = bget_path2(ft_cmd[0], env);
	apply_quote_removal(ft_cmd);
	if (execve(path, ft_cmd, env) == -1)
	{
		ft_putstr_fd("solo: command not found: ", STDERR_FILENO);
		ft_putendl_fd(ft_cmd[0], 2);
		ft_free_tab(ft_cmd);
		exit(127);
	}
}
int	hall_monitor(t_minishell *t_m, size_t i)
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

int	ft_soloexec(t_minishell *t_m, size_t i, int c_int)
{
	t_cmd	*c;
	char	**nenv;

	c = &(t_m->cmds[i]);
	nenv = pipe_env(t_m);
	if (i < t_m->cmd_count)
		if(pipe(t_m->pipes_fd[i]) == -1)
			return (-1);
	signalignore(SIGINT);
	t_m->pid[i] = fork();
	if (t_m->pid[i] == -1)
		return (-1);
	if (!t_m->pid[i])
		child_molestor(t_m, c, i, c_int, nenv);
	else
		hall_monitor(t_m, i);
	free(nenv);
	return (1);
}

int	ft_waiter(t_minishell *t_m)
{
	size_t	i;

	i = 0;
	t_m->exstat = 0;
	while (i < (t_m->cmd_count))
	{
		waitpid(t_m->pid[i], &t_m->exstat, 0);
		if(WIFSIGNALED(t_m->exstat))
		{
			t_m->exstat = 128 + WTERMSIG(t_m->exstat);
			write(2, "\n", 1);
		}
		i++;
	}
	return (0);
}

int	exec_init(t_minishell *t_m)
{
	t_m->pid = ft_calloc(t_m->cmd_count + 1, sizeof(pid_t));
	if (!t_m->pid)
		return (perror("pid array creation error."), exit(1), -1);
	t_m->pipes_fd = ft_calloc(t_m->cmd_count + 1, sizeof(int [2]));
	if (!t_m->pipes_fd)
		return (perror("pipes array creation error."), exit(1), -1);
	return (0);
}

int	executions(t_minishell *t_m, size_t i)
{
	t_cmd	*c;
	int		c_int;

	c = &(t_m->cmds[i]);
	if (c->command == NULL)
			ft_soloexec(t_m, i, -1);
	else
	{
		t_m->c_args = ft_split_quotes(c->command, ' ', 0);
		c_int = is_builtin(rm_quotes(t_m->c_args[0]));
		if (c_int != -1 && !c->is_piped_in && !c->is_piped_out)
			builtindirector(t_m, c, c_int);
		else
			ft_soloexec(t_m, i, c_int);
		ft_free_tab(t_m->c_args);
		//fprintf(stderr, "command: &%s&\n", c->command);
	}
	return (0);
}

void	exec_cmds(t_minishell *t_m)
{
	size_t	i;

	i = 0;
	exec_init(t_m);
	if (heredocalloc(t_m) != -1)
		while (i < t_m->cmd_count)
			executions(t_m, i++);
	else
		write(2,"\n",1);
	ft_waiter(t_m);
	delete_heredocs(t_m);
	signalsetter(SIGINT, handler);
}
