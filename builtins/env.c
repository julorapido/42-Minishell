/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:23:06 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/06 16:41:05 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	f__env(t_env *env, int fdout)
{
	while (env && env->next != NULL)
	{
		if(FM(env->value, '=', '=') != -1)
			ft_putendl_fd(env->value, fdout);
		env = env->next;
	}
	if (env && FM(env->value, '=', '=') != -1)
		ft_putendl_fd(env->value, fdout);
	return (SUCCESS);
}
