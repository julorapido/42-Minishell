/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_police.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:41:20 by gchauvot          #+#    #+#             */
/*   Updated: 2024/11/29 16:43:32 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	closepipesonfail(t_minishell *t_m, t_cmd *c, int i)
{
	close(t_m->pipes_fd[i][0]);
	close(t_m->pipes_fd[i][1]);
	if (i > 0)
	{
		if (!c->is_piped_in)
			close(t_m->pipes_fd[i - 1][0]);
		if (!c->is_piped_in)
			close(t_m->pipes_fd[i - 1][1]);
	}
	exit(1);
}

int	child_molestor(t_minishell *t_m, t_cmd *c, size_t i, int c_int, char **nenv)
{
	signalsetter(SIGINT, SIG_DFL);
	if (childhead_handler(t_m, i, c))
		closepipesonfail(t_m, c, i);
	// fermeture du precedent pipe dans le current child dans le cas ou
	if (i > 0)
	{
		if (!c->is_piped_in)
			close(t_m->pipes_fd[i - 1][0]);
		if (!c->is_piped_in)
			close(t_m->pipes_fd[i - 1][1]);
	}
	// exec builtin dans ce child dans le cas ou le builtin est dans un pipe
	if (c_int != -1)
		run_builtin(t_m, c_int, 1, c);
	else if (c->command)
		ft_exec2(c->command, nenv);
	exit(0);
}

int	childhead_handler(t_minishell *t_m, size_t i, t_cmd *c)
{
	int	fd;
	int	a;

	a = 0;
	while (a < c->f_i)
	{
		fd = open_file(c->files[a].f_name, c->files[a]._out, c->files[a].append);
		if (fd == -1)
			return (-1);
		dupclose(fd, c->files[a]._out);
		a++;
	}
	if (i > 0 && c->is_piped_in)
	{
		dupclose(t_m->pipes_fd[i - 1][0], 0);
		close(t_m->pipes_fd[i - 1][1]);
	}
	close(t_m->pipes_fd[i][0]);
	if (c->is_piped_out)
		dupclose(t_m->pipes_fd[i][1], 1);
	else
		close(t_m->pipes_fd[i][1]);
	return (0);
}