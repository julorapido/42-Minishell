/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:45:02 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/08 12:40:28 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_init(t_minishell *t_m, char **argv)
{
	t_env	*env;
	t_env	*new;
	int		i;

	if (!(env = malloc(sizeof(t_env))))
		return (1);
	env->value = ft_strdup(argv[0]);
	env->next = NULL;
	t_m->env = env;
	i = 1;
	while (argv && argv[0] && argv[i])
	{
		if (!(new = malloc(sizeof(t_env))))
			return (1);
		new->value = ft_strdup(argv[i]);
		new->next = NULL;
		env->next = new;
		env = new;
		i++;
	}
	return (0);
}
