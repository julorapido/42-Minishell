/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchauvot <gchauvot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:23:06 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/16 15:39:49 by gchauvot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		f__env(t_env *env, int fdout)
{
	while (env && env->next != NULL)
	{
		ft_putendl_fd(env->value, fdout);
		env = env->next;
	}
	if (env)
		ft_putendl_fd(env->value, fdout);
	return (SUCCESS);
}

