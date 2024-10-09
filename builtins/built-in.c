/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:15:52 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/09 15:04:58 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
	return (-1);
}

void	run_builtin(t_minishell *t_m, int n_builtin)
{
	if (n_builtin == 1)
		f__echo(t_m->c_args);
	if (n_builtin == 3)
		f__pwd();
	if (n_builtin == 4)
		f__env(t_m->env);
	if (n_builtin == 6)
		f__unset(t_m);
}
