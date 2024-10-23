/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchauvot <gchauvot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:08:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/23 16:15:57 by gchauvot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void shlvladd(char *env)
{
	size_t	j;
	char	*res;
	j = 0;
	if(!ft_strncmp(env, "SHLVL=", 6))
	{
		res = ft_itoa(ft_atoi(&env[6]) + 1 ) ;
		while (j < ft_strlen(res))
		{
			env[6 + j] = res[j];
			j++;
		}
		env[6 + j] = '\0';
	}
	if(!ft_strcmp(env, "SHELL=/bin/zsh"))
	{
		// printf("brosat,env: $%s$\n", env);
		// // char *lolo=ft_strdup("SHELL=/home/gchauvot/Proj/Minishells/v8/MINISHELL");
		// char *lolo="SHELL=/home/gchauvot/Proj/Minishells/v8/MINISHELL";
		// strcat(env,lolo);
		// printf("brosat,env: $%s$\n", env);
	}

}

void	shlvlhandler(char **env)
{
	int		i;
	size_t	j;
	char	*res;
	char	*pwd;

	i = 0;
	j = 0;
	while(env[i])
	{	
		if(!ft_strncmp(env[i], "SHLVL=", 6))
		{
			res = ft_itoa(ft_atoi(&env[i][6]) + 1) ;
			while (j < ft_strlen(res))
			{
				env[i][6 + j] = res[j];
				j++;
			}
			free(res);
			env[i][6+j] = '\0';
		}
		if(!ft_strcmp(env[i], "SHELL=/bin/zsh"))
		{
			pwd = ft_strdup(getenv("_"));
			char *temp=ft_strjoin("SHELL=", pwd);
			free(pwd);
			env[i] = temp;
		}
		i++;
	}
}
void	ft_exec2(char *cmd, char **env)
{
	char	**ft_cmd;
	char	*path;

	ft_cmd = ft_split(cmd, ' ');
	path = bget_path2(ft_cmd[0], env);
	signalsetter(SIGINT, SIG_DFL);		
	if (execve(path, ft_cmd, env) == -1)
	{
		ft_putstr_fd("solo: command not found: ", STDERR_FILENO);
		ft_putendl_fd(ft_cmd[0], 2);
		ft_free_tab(ft_cmd);
		exit(0);
	}
}

int	ft_soloexec(t_minishell *t_m, size_t i, int c_int)
{
	t_cmd	*c;
	char	**outlist;
	char 	**nenv;

	c = &(t_m->commands[i]);
	outlist = ft_split(c->output,' ');
	nenv = pipe_env(t_m);
	if (!ft_strcmp(c->output, "pipe"))
		pipe(t_m->pipes_fd[i]);
	signalignore(SIGINT);
	t_m->pid[i] = fork();
	if(!t_m->pid[i])
		child_molestor(t_m, c, i, c_int,outlist, nenv);
	if(i>0)
	{
		c = &(t_m->commands[i-1]);
		if(!ft_strcmp(c->output, "pipe"))
			close(t_m->pipes_fd[i-1][0]);
		if(!ft_strcmp(c->output, "pipe"))
			close(t_m->pipes_fd[i-1][1]);
	}
	free(nenv);
	ft_free_tab(outlist);
	return (1);
}

int	nullcommand(t_minishell *t_m, size_t i)
{
	ft_soloexec(t_m, i, -1);
	return (0);
}

int	ft_waiter(t_minishell *t_m)
{
	size_t	i;
	int		pid_exnum;
	int		balance;

	i = 0;
	pid_exnum = 0;
	balance = 0;
	while(i < (t_m->cmd_count))
	{
		waitpid(t_m->pid[i], &pid_exnum, 0);
		if(pid_exnum == 2 && balance == 0)
		{
			write(2,"\n",1);
			balance = 1;
		}
		i++;
	}
	return (0);
}
void	exec_cmds(t_minishell *t_m)
{
	t_cmd	*c;
	size_t	i;
	int		c_int;
	

	i = 0;
	fprintf(stderr,"--------- COMMANDS EXECUTION ---------\n");
	t_m->pid = ft_calloc(t_m->cmd_count + 1, sizeof(pid_t));
	if (!t_m->pid)
		return (perror("pid array creation error."), exit(1));
	t_m->pipes_fd = ft_calloc(t_m->cmd_count + 1, sizeof(int[2]));
	if (!t_m->pipes_fd)
		return (perror("pipes array creation error."), exit(1));
	if(heredocalloc(t_m) == -1)
		return ;
	while(i < t_m->cmd_count )
	{
		c = &(t_m->commands[i]);
		if(c->command == NULL)
		{
			//FREE C_ARGS impossible si command nulle;
			//fprintf(stderr, "c->command: $%s$, i: $%ld$\n", c->command, i);
			ft_soloexec(t_m, i, -1);
			i++;
			continue ;
		}
		t_m->c_args = ft_split(c->command, ' ');
		c_int = is_builtin(t_m->c_args[0]);
		if (c_int != -1 && ft_strcmp(c->input, "pipe") && ft_strcmp(c->output, "pipe"))
			run_builtin(t_m, c_int, 1);
		else
			ft_soloexec(t_m, i, c_int);
		i++;
		free_c_args(t_m);
	}
	ft_waiter(t_m);
	signalsetter(SIGINT, handler);
	delete_heredocs(t_m);
	free(t_m->pipes_fd);
	free(t_m->pid);
	//free(t_m->heredocs);
}
