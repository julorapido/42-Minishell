/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:08:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/08 14:44:41 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
		c_int = is_builtin(c);
		if(c_int != -1)
		{
			run_builtin(t_m, c_int);
		}
		else
		{
			// run pipe, execve, etc..
		}	
		i--;
	}
}
