/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:30:09 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/28 15:12:02 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	f__exit(t_minishell *t_m, t_cmd *c)
{
	char **tmp;

	tmp = ft_split(c->command, ' ');
	if (tmp[1])
		t_m->exstat = ft_atoi(tmp[1]);
	//fprintf(stderr, "exit status tmp[1]: %s, t_,exsitat: %d\n", tmp[1], t_m->exstat);
	ft_free_tab(tmp);
	free_c_args(t_m);
	delete_heredocs(t_m);
	free(t_m->pipes_fd);
	free(t_m->pid);
	exit(t_m->exstat);
}
