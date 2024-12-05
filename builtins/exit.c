/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:30:09 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/05 17:38:24 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_c_args(t_minishell	*t_m)
{
	int	i;

	i = 0;
	while (t_m->c_args[i])
	{
		free(t_m->c_args[i]);
		i++;
	}
	free(t_m->c_args[i]);
	free(t_m->c_args);
}

static int	is_numeric_operand(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (ft_isalpha(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	f__exit(t_minishell *t_m, t_cmd *c)
{
	char	**tmp;
	int		e;

	e = (0);
	tmp = ft_split(c->command, ' ');
	if (tmp[1])
	{
		if (!is_numeric_operand(tmp[1]))
			e = 2;
		else if (FM(tmp[1], '-', '-') != -1)
			e = 156;
		else
			e = ft_atoi(ft_str_remvchr(tmp[1], '\"', '\"'));
		if (tmp[2])
		{
			e = 1;
			ft_putstr_fd(" too many arguments\n", 2);
		}
	}
	ft_free_tab(tmp);
	free_c_args(t_m);
	delete_heredocs(t_m);
	free(t_m->pipes_fd);
	free(t_m->pid);
	return (e);
}
