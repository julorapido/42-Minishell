/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchauvot <gchauvot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:08:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/14 15:08:53 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void free_c_args(t_minishell	*t_m)
{
	int		i;

	i = 0;
	while (t_m->c_args[i])
	{
		free(t_m->c_args[i]);
		i++;
	}
	free(t_m->c_args[i]);
}


char	**pipe_env(t_minishell *t_m)
{
	int	i;
	int	x;
	char **env;
	t_env *temp;

	i = 0;
	x = 0;
	temp = t_m->env;
	while (t_m->env)
	{
		i++;
		t_m->env = t_m->env->next;
	}
	env = ft_calloc(i + 1, sizeof(char *));
	if (!env)
		return (NULL);
	t_m->env = temp;
	while (x < i)
	{
		env[x] = t_m->env->value;
		t_m->env = t_m->env->next;
		x++;
	}
	t_m->env = temp;
	return (env);
}

// adapt cmd list to old pipex
int	pipe_arg(t_pipe *piper, t_minishell *t_m, int n_cmd, int start)
{
	char	**args;
	char	**env;
	int		i;
	int		x;

	i = 0;
	x = 0;
	printf("start: %d, ncmd: %d\n", start, n_cmd);
	args = ft_calloc(n_cmd + 3, sizeof(char *));
	if (!args)
		return (0);
	args[0] = "minishell";
	// args[1] = t_m->commands[start].input;
	// args[n_cmd + 2] = t_m->commands[start-i].output;
	args[1] = "Makefile";
	args[n_cmd + 2] = "/dev/stdout";
	while (i < n_cmd)
	{
		args[i + 2] = t_m->commands[start + i].command;
		i++;
	}
	env = pipe_env(t_m);
	piper->argc = n_cmd + 3;
	piper->argv = args;
	piper->envp = env;
	return (1);
}


int	pipe_parser(t_minishell *t_m, int i)
{
	int start;
	t_cmd	*c;
	t_pipe	piper;

	start = i;
	c = &(t_m->commands[i]);
	//printf("c[%d]: @%s@\n",i, c->command);
	while (!ft_strcmp(c->output, "pipe"))
	{
		c = &(t_m->commands[i]);
		//printf("c[%d]: @%s@\n",i, c->command);
		i++;
	}
	pipe_arg(&piper, t_m, i - start, start);
	pipex(piper.argc, piper.argv, piper.envp, t_m);
	free(piper.argv);
	free(piper.envp);
	return(i);
}

void	ft_exec2(char *cmd, char **env, pid_t pid, t_minishell *t_m)
{
	char	**ft_cmd;
	char	*path;
	int		built;

	ft_cmd = ft_split(cmd, ' ');
	built = is_builtin(cmd);
	path = bget_path2(ft_cmd[0], env);
	if(built != -1)
	{
		run_builtin(t_m, built);
		exit(0);
	}
	if (execve(path, ft_cmd, env) == -1)
	{
		ft_putstr_fd("pipex: command not found: ", STDERR_FILENO);
		ft_putendl_fd(ft_cmd[0], 2);
		ft_free_tab(ft_cmd);
		//free(&pid);
		exit(0);
	}
}

int	ft_soloexec(t_minishell *t_m, int i)
{
	t_cmd	*c;
	pid_t	pid;
	int		fdin;
	int		fdout;
	int		x;
	char	**outlist;

	x = 0;
	c = &(t_m->commands[i]);
	outlist = ft_split(c->output,' ');
	while (x <= c->n_redirections)
	{
		x++;
		pid = fork();
		if(!pid)
		{
			if (ft_strcmp(c->input, "STD_IN"))
			{
				fdin = open_file(c->input, 0);
				dup2(fdin, 0);
				close(fdin);
			}
			if (ft_strcmp(c->output, "STD_OUT"))
			{
				fdout = open_file(outlist[x], 1);
				dup2(fdout, 1);
				close(fdout);
			}
			ft_exec2(c->command, pipe_env(t_m), pid, t_m);	
			exit(1);
		}
		waitpid(pid, 0, 0);
	}
	return (1);
}

void	exec_cmds(t_minishell *t_m)
{
	t_cmd	*c;
	int		i;
	int		c_int;

	i = 0;
	printf("--------- COMMANDS EXECUTION ---------\n");
	while(i <= t_m->cmd_count - 1)
	{
		c = &(t_m->commands[i]);
		if(!c->command)
			break;
		//printf("PARRENT c[%d]: @%s@\n",i, c->command);
		t_m->c_args = ft_split(c->command, ' ');	
		//if(ft_strcmp(t_m->c_args[0], "echo") == 0)
		//	c->command = ft_strdup(t_m->c_args[0]);
		c_int = is_builtin(t_m->c_args[0]);

		// if pipe: 
		if (!ft_strcmp(c->output, "pipe"))
		{
			//printf("PAR2RENT c[%d]: @%s@\n",i, c->command);
			i += pipe_parser(t_m, i);
			free_c_args(t_m);
			continue ;
		}
		
		// if(c_int != -1)
		// {
		// 	run_builtin(t_m, c_int);
		// }
		else
		{
			ft_soloexec(t_m, i);
		}
		//fprintf(stderr, "--------- COMMANDS otast i: %d ---------\n", i);
		i++;
		free_c_args(t_m);
	}
}
