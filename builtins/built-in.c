/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:15:52 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/09 17:56:02 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
static int	openfds(t_cmd *c, int fd_stds[2], int fd_duped[2])
{
	int	a;

		while (a < c->f_i)
	{
		fd = open_file(c->files[a].f_name,
				c->files[a]._out, c->files[a].append);
		if (fd == -1)
			return (1);
		dupclose(fd, c->files[a]._out);
		a++;
	}
	if (c->n_in > 0)
	{
		fd_stds[0] = dup(0);
		fd_duped[0] = open_file((LF_(0, c))->f_name, 0, 0);
		if (fd_duped[0] == -1)
			return (-1);
		dup2(fd_duped[0], 0);
		close(fd_duped[0]);
	}
	if (c->n_out > 0)
	{
		fd_stds[1] = dup(1);
		fd_duped[1] = open_file((LF_(1, c))->f_name, 1, (LF_(0, c))->append);
		if (fd_duped[1] == -1)
			return (-1);
		dup2(fd_duped[1], 1);
		close(fd_duped[1]);
	}
	return (0);
}*/

int	restorefds(t_cmd *c, int fd_stds[2])
{
	if (c->n_in > 0)
	{
		dup2(fd_stds[0], 0);
		close(fd_stds[0]);
	}
	if (c->n_out > 0)
	{
		dup2(fd_stds[1], 1);
		close(fd_stds[1]);
	}
	return (0);
}

int	openfds(t_minishell *t_m, t_cmd *c, int fd_stds[2], int fd_duped[2])
{
	int		a;

	a = 0;
	while (a < c->f_i)
	{
		fd_stds[c->files[a]._out] = dup(c->files[a]._out);
		fd_duped[c->files[a]._out] = open_file(ft_rm_quotes(c->files[a].f_name), c->files[a]._out, c->files[a].append);
		if (fd_duped[c->files[a]._out] == -1)
		{
			t_m->exstat = 1;
			return (-1);
		}
		dup2(fd_duped[c->files[a]._out], c->files[a]._out);
		close(fd_duped[c->files[a]._out]);
		a++;
	}
	return (0);
}


int	builtindirector(t_minishell *t_m, t_cmd *c, int n_builtin)
{
	int	fd_stds[2];
	int	fd_duped[2];


	if (openfds(t_m, c, fd_stds, fd_duped) == -1)
	{
		restorefds(c, fd_stds);
		fprintf(stderr, "joe\n");
		return (-1);
	}
	run_builtin(t_m, n_builtin, fd_duped[1], c);
	fprintf(stderr, "qwkwqw\n");
	restorefds(c, fd_stds);
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

int	run_builtin(t_minishell *t_m, int n_builtin, int fdout, t_cmd *cmd)
{
	int	a;

	if (n_builtin == 1)
		a = f__echo(t_m->c_args, fdout);
	if (n_builtin == 2)
		a = f__cd(t_m->c_args, t_m);
	if (n_builtin == 3)
		a = f__pwd(fdout);
	if (n_builtin == 4)
		a = f__env(t_m->env, fdout);
	if (n_builtin == 5)
		a = f__export(t_m);
	if (n_builtin == 6)
		a = f__unset(t_m);
	if (n_builtin == 7)
		a = f__exit(t_m, cmd);
	return (a);
}
