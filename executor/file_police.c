/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_police.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:41:20 by gchauvot          #+#    #+#             */
/*   Updated: 2024/11/29 11:51:21 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	child_molestor(t_minishell *t_m, t_cmd *c, size_t i, int c_int,
	char **outlist, char **nenv)
{
	signalsetter(SIGINT, SIG_DFL);
	childhead_handler(t_m, i, c);
	childbutt_handler(t_m, i, c, outlist);
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
		ft_exec2(c->command, nenv,t_m);
	exit(0);
}

int	childhead_handler(t_minishell *t_m, size_t i, t_cmd *c)
{
	int	fdin;
	
	if (!c->input)
		return (0);
	// ni STDIN ni PIPE en entree
	if (!c->is_stdin && !c->is_piped_in
		&& c->input)
	{
		fdin = open_file(c->input, 0, 0);
		dupclose(fdin, 0);
	}
	// entree de la PIPE
	else if (i>0 && c->is_piped_in)
	{
		dupclose(t_m->pipes_fd[i - 1][0], 0);
		close(t_m->pipes_fd[i - 1][1]);
	}
	return (0);
}

int	childbutt_handler(t_minishell *t_m, size_t i, t_cmd *c, char **outlist)
{
	
	int		fdout;
	int		x;
	int		n_out;

	x = 0;
	if (c->n_redirections)
		n_out = c->n_redirections - 1;
	else
		n_out = c->n_redirections;
	if (!c->output)
		return (-1);
	if (!c->is_stdout && !c->is_piped_out)
	{
		while (x <= n_out)
		{
			fdout = open_file(outlist[x], 1, c->is_append);
			dupclose(fdout, 1);
			x++;
		}
	}
	close(t_m->pipes_fd[i][0]);
	if (c->is_piped_out)
		dupclose(t_m->pipes_fd[i][1], 1);
	else
		close(t_m->pipes_fd[i][1]);
	
	return (0);
}
