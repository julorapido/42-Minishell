/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:08:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/07 14:04:54 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	exec_cmds(t_minishell *t_m)
{
	t_cmd	*c;
	int		i;
	int		c_int;

	c = &(*t_m->commands);
	while(c)
	{
		c_int = is_builtin(c.command);
		if(is_builtin)
			run_builtin(t_m->env, c_int);
		}else
		{
			// run pipe, execve, etc..
		}	
		i++;
		c = &(t_m->commands[i]);
	}
}
