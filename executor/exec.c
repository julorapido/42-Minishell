/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:08:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/09 15:04:00 by jsaintho         ###   ########.fr       */
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


void	exec_cmds(t_minishell *t_m)
{
	t_cmd	*c;
	int		i;
	int		c_int;

	i = t_m->cmd_count - 1;
	printf("--------- COMMANDS EXECUTION ---------\n");
	while(i >= 0)
	{
		c = &(t_m->commands[i]);
		if(!c->command)
			break;
		t_m->c_args = ft_split(c->command, ' ');	
		//if(ft_strcmp(t_m->c_args[0], "echo") == 0)
		//	c->command = ft_strdup(t_m->c_args[0]);
		c_int = is_builtin(t_m->c_args[0]);
		if(c_int != -1)
		{
			run_builtin(t_m, c_int);
		}
		else
		{
			// run pipe, execve, etc..
		}	
		i--;
		free_c_args(t_m);
	}
}
