/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_police.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchauvot <gchauvot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:41:20 by gchauvot          #+#    #+#             */
/*   Updated: 2024/10/23 15:29:19 by gchauvot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	child_molestor(t_minishell *t_m, t_cmd *c, size_t i, int c_int,
	char **outlist, char **nenv)
{
	childhead_handler(t_m, i, c);
	childbutt_handler(t_m, i, c, outlist);
	// fermeture du precedent pipe dans le current child dans le cas ou
	// le pipe actuel est ecrase par un heredoc
	if(i > 0 )//&& c->is_heredoc)
	{
		t_cmd *ac = &(t_m->commands[i-1]);
		if(!ft_strcmp(ac->output, "pipe") && ft_strcmp(c->input, "pipe"))
			close(t_m->pipes_fd[i-1][0]);
		if(!ft_strcmp(ac->output, "pipe") && ft_strcmp(c->input, "pipe"))
			close(t_m->pipes_fd[i-1][1]);
	}
	// exec builtin dans ce child dans le cas ou le builtin est dans un pipe
	if (c_int != -1)
		run_builtin(t_m, c_int, 1);
	else if (c->command)
		ft_exec2(c->command, nenv);
	close(1);
	close(2);
	close(0);
	fprintf(stderr, "jojo\n");
	exit(1);
}

int childhead_handler(t_minishell *t_m, size_t i, t_cmd *c)
{
	int	fdin;

	if (!c->input)
		return (0);
	// ni STDIN ni PIPE en entree
	if (ft_strcmp(c->input, "STD_IN") && ft_strcmp(c->input, "pipe") && c->input)
	{
		fdin = open_file(c->input, 0, 0);
		dupclose(fdin, 0);
	}
	// entree de la PIPE		
	else if (!ft_strcmp(c->input, "pipe"))
	{
		fprintf(stderr, "child head: if2\n");
		dupclose(t_m->pipes_fd[i - 1][0], 0);
		close(t_m->pipes_fd[i - 1][1]);
	}
	return(0);
}

int childbutt_handler(t_minishell *t_m, size_t i, t_cmd *c, char **outlist)
{
	int fdout;
	int		x;
	int		n_out;
	
	x = 0;
	if (c->n_redirections)
		n_out = c->n_redirections-1;
	else
		n_out = c->n_redirections;
	// ni STDOUT ni PIPE en sortie
	if (!c->output)
		return (0);
	if (ft_strcmp(c->output, "STD_OUT") && ft_strcmp(c->output, "pipe"))
	{		
		while(x <= n_out)
		{
			fdout = open_file(outlist[x], 1, c->appends[x]);
			dup2(fdout, 1);
			close(fdout);
			x++;
		}
	}
	// sortie de la PIPE
	if (!ft_strcmp(c->output, "pipe"))
	{
		close(t_m->pipes_fd[i][0]);
		dupclose(t_m->pipes_fd[i][1], 1);
	}
	return(0);
}

// old one

// int	child_molestor(t_minishell *t_m, t_cmd *c, int i, int c_int,
// 	char **outlist, char **nenv)
// {
// 	int		fdin;
// 	int		fdout;
// 	int		x;
// 	int		n_out;
	
// 	if (c->n_redirections)
// 		n_out = c->n_redirections-1;
// 	else
// 		n_out = c->n_redirections;

// 	x = 0;
// 	// ni STDIN ni PIPE en entree
// 	if (ft_strcmp(c->input, "STD_IN") && ft_strcmp(c->input, "pipe"))
// 	{
// 		if (c->is_heredoc)
// 			{
// 				char *fname = ft_strjoin("temp/", ft_itoa(i));
// 				fdin = open_file(fname, 0);
// 			}
// 		else
// 			fdin = open_file(c->input, 0);
// 		if (dup2(fdin, 0) == -1)
// 			return (perror("minishell: "), exit(1), 1);
// 		if (close(0) == -1)
// 			return (perror("minishell: "), exit(1), 1);
// 		//dupclose(fdin, 0);

// 	}

// 	// ni STDOUT ni PIPE en sortie
// 	if (ft_strcmp(c->output, "STD_OUT") && ft_strcmp(c->output, "pipe"))
// 	{		
// 		while(x <= n_out)
// 		{
// 			fdout = open_file(outlist[x], 1);
// 			dup2(fdout, 1);
// 			close(fdout);
// 			x++;
// 		}
// 	}

// 	// PIPE du milieu, pour les gourverner tous
// 	if (!ft_strcmp(c->input, "pipe") && !ft_strcmp(c->output, "pipe"))
// 	{
// 		dup2(t_m->pipes_fd[i - 1][0], 0); // dup read from previous
// 		dup2(t_m->pipes_fd[i][1], 1);	// dup writefornext
// 		close(t_m->pipes_fd[i - 1][0]);
// 		close(t_m->pipes_fd[i][1]);
// 		close(t_m->pipes_fd[i][0]);
// 	}

// 	// entree du DERNIER PIPE		
// 	if (!ft_strcmp(c->input, "pipe") && ft_strcmp(c->output, "pipe"))
// 	{
		
// 		dup2(t_m->pipes_fd[i - 1][0], 0);
// 		close(t_m->pipes_fd[i - 1][0]);
// 		close(t_m->pipes_fd[i - 1][1]);
// 	}

// 	// sortie du PREMIER PIPE
// 	if (ft_strcmp(c->input, "pipe") && !ft_strcmp(c->output, "pipe"))
// 	{
// 		close(t_m->pipes_fd[i][0]);
// 		dup2(t_m->pipes_fd[i][1], 1);
// 		close(t_m->pipes_fd[i][1]);
// 	}
	
// 	// fermeture du precedent pipe dans le current child dans le cas ou
// 	// le pipe actuel est ecrase par un heredoc
// 	if(i > 0 && c->is_heredoc)
// 	{
// 		t_cmd *ac = &(t_m->commands[i-1]);
// 		if(!ft_strcmp(ac->output, "pipe") && ft_strcmp(c->input, "pipe"))
// 			close(t_m->pipes_fd[i-1][0]);
// 		if(!ft_strcmp(ac->output, "pipe") && ft_strcmp(c->input, "pipe"))
// 			close(t_m->pipes_fd[i-1][1]);
// 	}

// 	// exec builtin dans ce child dans le cas ou le builtin est dans un pipe
// 	if (c_int != -1)
// 		run_builtin(t_m, c_int, 1);
// 	else // execute la fonction ! enfin TT
// 		ft_exec2(c->command, nenv, t_m->pid[i], t_m);
// 	exit(1);
// }
