/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:08:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/20 17:26:47 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shlvlhandler(char **env)
{
	int		i;
	size_t	j;
	int		mark;
	char	*res;

	i = 0;
	j = 0;
	mark = 0;
	while (env[i])
	{	
		if (!ft_strncmp(env[i], "SHLVL=", 6))
		{
			res = ft_itoa(ft_atoi(&env[i][6]) + 1);
			while (j < ft_strlen(res))
			{
				env[i][6 + j] = res[j];
				j++;
			}
			free(res);
			env[i][6 + j] = '\0';
			mark = -1;
		}
		i++;
	}
	if (mark != -1)
	{
		env[i] = "SHLVL=0";
		env[i + 1] = 0;
	}
}

void	ft_exec2(char *cmd, char **env, t_minishell *tm)
{
	char	**ft_cmd;
	char	*path;

	ft_cmd = ft_split(cmd, ' ');
	//trim_tab(ft_cmd, tm);
	path = bget_path2(ft_cmd[0], env);
	if (execve(path, ft_cmd, env) == -1)
	{
		ft_putstr_fd("solo: command not found: ", STDERR_FILENO);
		ft_putendl_fd(ft_cmd[0], 2);
		ft_free_tab(ft_cmd);
		exit(127);
	}
}

int	ft_soloexec(t_minishell *t_m, size_t i, int c_int)
{
	t_cmd	*c;
	char	**outlist;
	char	**nenv;
	int 	pass=0;

	c = &(t_m->commands[i]);
	outlist = ft_split(c->output, ' ');
	nenv = pipe_env(t_m);
	if (i < t_m->cmd_count)//c->is_piped_out)
		pipe(t_m->pipes_fd[i]);
	signalignore(SIGINT);
	t_m->pid[i] = fork();
	if (!t_m->pid[i])
		child_molestor(t_m, c, i, c_int, outlist, nenv);
	if (i > 0)
	{
		c = &(t_m->commands[i - 1]);
			close(t_m->pipes_fd[i - 1][0]);
		if (pass==0)
			close(t_m->pipes_fd[i - 1][1]);
	}
	if (i == t_m->cmd_count-1)
	{
		c = &(t_m->commands[i]);
		close(t_m->pipes_fd[i][0]);
		close(t_m->pipes_fd[i][1]);
	}
	free(nenv);
	ft_free_tab(outlist);
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
			t_m->exstat = WEXITSTATUS(t_m->exstat);
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

	c = &(t_m->commands[i]);
	if (c->command == NULL)
			ft_soloexec(t_m, i, -1);
	else if (t_m->is_expand && ft_strchr(c->command, '=') && !ft_strchr(c->command, ' '))
	{
		free(c->command);
		c->command = NULL;
		ft_soloexec(t_m, i, -1);
	}
	else
	{
		t_m->c_args = ft_split(c->command, ' ');
		c_int = is_builtin(t_m->c_args[0]);
		if (c_int != -1 && !c->is_piped_in && !c->is_piped_out)
			builtindirector(t_m, c, c_int);
		else
			ft_soloexec(t_m, i, c_int);
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
