/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:15:52 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/02 17:15:05 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	restorefds(t_cmd *c, int fd_stds[2])
{
	if (!c->is_piped_in)
	{
		dup2(fd_stds[0], 0);
		close(fd_stds[0]);
	}
	if (!c->n_out)
	{
		dup2(fd_stds[1], 1);
		close(fd_stds[1]);
	}
	return (0);
}

int	openfds(t_cmd *c, int fd_stds[2], int fd_duped[2])
{

	if (c->n_in > 0)
	{
		printf("N PIPED IN \n");
		fd_stds[0] = dup(0);
		fd_duped[0] = open_file(c->input, 0, 0);
		if (fd_duped[0] == -1)
			return (-1);
		dup2(fd_duped[0], 0);
		close(fd_duped[0]);
	}
	if (!c->n_out)
	{
		printf("N N_OUT \n");
		fd_stds[1] = dup(1);
		fd_duped[1] = open_file(c->output, 1, c->files[c->f_i-1].append);
		if (fd_duped[0] == -1)
			return (-1);
		dup2(fd_duped[1], 1);
		close(fd_duped[1]);
	}
	return (0);
}

int	builtindirector(t_minishell *t_m, t_cmd *c, int n_builtin)
{
	int fd_stds[2];
	int	fd_duped[2];

	//fprintf(stderr, "boolin: %d, boolout: %d, isappend; %d\n", c->is_stdin, c->is_stdout, c->is_append);
	if(openfds(c, fd_stds, fd_duped) == -1)
	//	return (restorefds(c, fd_stds), -1);
	run_builtin(t_m, n_builtin, 1, c);
	//restorefds(c, fd_stds);
	return (0);
}

int	is_builtin(char *c)
{
	if (ft_strcmp(c, "echo") == 0)
		return (1);
	if (ft_strcmp(c, "cd") == 0)
		return (2);
	if (ft_strcmp(c, "pwd") == 0)
		return (3);
	if (ft_strcmp(c, "env") == 0)
		return (4);
	if (ft_strcmp(c, "export") == 0)
		return (5);
	if (ft_strcmp(c, "unset") == 0)
		return (6);
	if (ft_strcmp(c, "exit") == 0)
		return (7);
	return (-1);
}

void	run_builtin(t_minishell *t_m, int n_builtin, int fdout, t_cmd *cmd)
{
	if (n_builtin == 1)
		f__echo(t_m->c_args, fdout);
	if (n_builtin == 2)
		f__cd(t_m->c_args, t_m);
	if (n_builtin == 3)
		f__pwd(fdout);
	if (n_builtin == 4)
		f__env(t_m->env, fdout);
	if (n_builtin == 5)
		f__export(t_m, fdout);
	if (n_builtin == 6)
		f__unset(t_m);
	if (n_builtin == 7)
		f__exit(t_m, cmd);
}
