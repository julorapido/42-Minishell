/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:08:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/17 13:22:25 by jsaintho         ###   ########.fr       */
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

int	open_file(char *file, int in_out)
{
	int	ret;

	if (in_out == 0)
		ret = open(file, O_RDONLY, 0777);
	if (in_out == 1)
		ret = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (ret == -1)
	{
		ft_putstr_fd("pipex: No such file or directory: ", 2);
		// fprintf(stderr, "errfile: %s\n", file);
		ft_putendl_fd(file, 2);
		exit(0);
	}
	return (ret);
}

char	*get_env(char **env)
{
	int			i;
	int			j;
	char		*s;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		s = ft_substr(env[i], 0, j);
		if (ft_strcmp(s, &("PATH"[0])) == 0)
		{
			free(s);
			return (env[i] + j + 1);
		}
		free(s);
		i++;
	}
	return (NULL);
}

char	*bget_path2(char *cmd, char **env)
{
	char	**b_path;
	char	**e_cmd;
	char	*exec_cmd;
	char	*str_path;
	int		i;

	i = -1;
	b_path = ft_split(get_env(env), ':');
	e_cmd = ft_split(cmd, ' ');
	while (b_path[++i])
	{
		str_path = ft_strjoin(b_path[i], "/");
		exec_cmd = ft_strjoin(str_path, e_cmd[0]);
		free(str_path);
		if (access(exec_cmd, F_OK | X_OK) == 0)
		{
			ft_free_tab(e_cmd);
			return (exec_cmd);
		}
		free(exec_cmd);
	}
	ft_free_tab(e_cmd);
	ft_free_tab(b_path);
	return (cmd);
}


void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
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
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	env = ft_calloc(i + 1, sizeof(char *));
	if (!env)
		return (NULL);
	temp = t_m->env;
	while (x < i)
	{
		env[x] = temp->value;
		temp = temp->next;
		x++;
	}
	return (env);
}

void	ft_exec2(char *cmd, char **env, pid_t pid, t_minishell *t_m)
{
	char	**ft_cmd;
	char	*path;

	ft_cmd = ft_split(cmd, ' ');
	path = bget_path2(ft_cmd[0], env);
	if (execve(path, ft_cmd, env) == -1)
	{
		ft_putstr_fd("solo: command not found: ", STDERR_FILENO);
		ft_putendl_fd(ft_cmd[0], 2);
		ft_free_tab(ft_cmd);
		exit(0);
	}
}


int	ft_soloexec(t_minishell *t_m, int i, int c_int)
{
	t_cmd	*c;
	int		fdin;
	int		fdout;
	int		x;
	char	**outlist;
	char 	**nenv;
	int		n_out;

	x = 0;
	fprintf(stderr,"soloexec\n");
	c = &(t_m->commands[i]);
	outlist = ft_split(c->output,' ');
	nenv = pipe_env(t_m);
	if (c->n_redirections)
		n_out = c->n_redirections-1;
	else
		n_out = c->n_redirections;
	while (x <=n_out)
	{
		if (!ft_strcmp(c->output, "pipe"))
			pipe(t_m->pipes_fd[i]);
		t_m->pid[i] = fork();
		if(!t_m->pid[i])
		{
			//fprintf(stderr, "voila: @%s@\n", c->input);
			if (ft_strcmp(c->input, "STD_IN") && ft_strcmp(c->input, "pipe"))
			{
				if (c->is_heredoc)
					{
						char *fname = ft_strjoin("temp/", ft_itoa(i));
						fdin = open_file(fname, 0);
						perror("heredocopen: ");
						fprintf(stderr, "bro: %d\n", fdin);
					}
				else
					fdin = open_file(c->input, 0);
				dup2(fdin, 0);
				perror("dup2: ");
				close(fdin);
				perror("close fdin ");

			}
			if (ft_strcmp(c->output, "STD_OUT") && ft_strcmp(c->output, "pipe"))
			{
				fdout = open_file(outlist[x], 1);
				dup2(fdout, 1);
				close(fdout);
			}
			if (!ft_strcmp(c->input, "pipe") && !ft_strcmp(c->output, "pipe"))
			{
				dup2(t_m->pipes_fd[i-1][0], 0); // dup read from previous
				dup2(t_m->pipes_fd[i][1], 1);	// dup writefornext
				close(t_m->pipes_fd[i-1][0]);
				close(t_m->pipes_fd[i][1]);
				close(t_m->pipes_fd[i][0]);
			}			
			if (!ft_strcmp(c->input, "pipe") && ft_strcmp(c->output, "pipe"))
			{
				
				dup2(t_m->pipes_fd[i - 1][0], 0);
				close(t_m->pipes_fd[i - 1][0]);
			}
			if (ft_strcmp(c->input, "pipe") && !ft_strcmp(c->output, "pipe"))
			{
				close(t_m->pipes_fd[i][0]);
				dup2(t_m->pipes_fd[i][1], 1);
				close(t_m->pipes_fd[i][1]);
			}
			
			if(i > 0 && c->is_heredoc)
			{
				t_cmd *ac = &(t_m->commands[i-1]);
				if(!ft_strcmp(ac->output, "pipe") && ft_strcmp(c->input, "pipe"))
					close(t_m->pipes_fd[i-1][0]);
				if(!ft_strcmp(ac->output, "pipe") && ft_strcmp(c->input, "pipe"))
					close(t_m->pipes_fd[i-1][1]);
			}

			if (c_int != -1)
				run_builtin(t_m, c_int, 1);
			else
				ft_exec2(c->command, nenv, t_m->pid[i], t_m);
			exit(1);
		}
		x++;
		if(i>0)
		{
			c = &(t_m->commands[i-1]);
			if(!ft_strcmp(c->output, "pipe"))
				close(t_m->pipes_fd[i-1][0]);
			if(!ft_strcmp(c->output, "pipe"))
				close(t_m->pipes_fd[i-1][1]);
		}
	}
	free(nenv);
	return (1);
}

int	nullcommand(t_minishell *t_m,t_cmd *c, int i)
{
	pid_t	pid;
	int		fdin;
	int		fdout;

	t_m->pid[i] = fork();
	if(t_m->pid[i] == -1)
		return(-1);
	if (!t_m->pid[i])
	{
		if (ft_strcmp(c->input, "STD_IN") && ft_strcmp(c->input, "pipe"))
		{
			fdin = open(c->input, O_RDONLY, 0642);
			if(fdin == -1)
			{
				perror("minishell: ");
				exit(1);
			}
			close(fdin);
		}
		if (ft_strcmp(c->output, "STD_OUT"))
		{
			fdout = open(c->output, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if(fdout == -1)
			{
				perror("minishell: ");
				exit(1);
			}
			close(fdout);
		}
		exit(1);
	}
}
void	exec_cmds(t_minishell *t_m)
{
	t_cmd	*c;
	int		i;
	int		c_int;

	i = 0;
	fprintf(stderr,"--------- COMMANDS EXECUTION ---------\n");
	fprintf(stderr,"cmdcount=@%ld@\n", t_m->cmd_count);
	t_m->pid = ft_calloc(t_m->cmd_count + 1, sizeof(pid_t));
	if (!t_m->pid)
		return (perror("pid array creation error."), exit(1));
	t_m->pipes_fd = ft_calloc(t_m->cmd_count + 1, sizeof(int[2]));
	if (!t_m->pipes_fd)
		return (perror("pipes array creation error."), exit(1));
	heredocalloc(t_m);
	while(i <= t_m->cmd_count - 1)
	{
		c = &(t_m->commands[i]);
		if(!c->command)
		{
			nullcommand(t_m, c, i);
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
	i = 0;
	while(i < (t_m->cmd_count))
		waitpid(t_m->pid[i++], 0, 0);
	delete_heredocs(t_m);
	free(t_m->pipes_fd);
	free(t_m->pid);
	free(t_m->heredocs);
}

