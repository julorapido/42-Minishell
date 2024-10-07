/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:15:52 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/07 14:10:52 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	is_builtin(char *command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (2);
	if (ft_strcmp(command, "pwd") == 0)
		return (3);
	if (ft_strcmp(command, "env") == 0)
		return (4);
	if (ft_strcmp(command, "export") == 0)
		return (5);
	if (ft_strcmp(command, "unset") == 0)
		return (6);
	return (-1);
}

void	run_builtin(t_minishell *t_m, int n_builtin)
{
	if (n_builtin == 3)		
		f__pwd();
	if (n_builtin == 4)
		f__env(t_m->env);
}
