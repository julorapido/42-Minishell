/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchauvot <gchauvot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:45:02 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/02 17:16:33 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	size_t_var(char *s)
{
	int i;

	i = 0;
	while(s[i] && s[i] != '=')
		i++;
	return (i);
}


char	*get_env_path(t_minishell *t_m)
{
	t_env	*n;
	char	**spl;
	char 	*v;
	int		j;

	v = NULL;
	n = (t_m->env);
	while (n)
	{
		if (ft_strncmp(n->value, "PATH", size_t_var(n->value)) == 0)
		{
			spl = ft_split(n->value, '=');
			v = spl[1];
			j = 0;
			while (spl[j])
			{
				free(spl[j]);
				j++;
			}
			break;
		}
		n = n->next;
	}
	return (v);
}


int	env_init(t_minishell *t_m, char **argv)
{
	t_env	*env;
	t_env	*new;
	int		i;

	if (!(env = ft_calloc(1, sizeof(t_env))))
		return (1);
	env->value = ft_strdup(argv[0]);
	env->next = NULL;
	t_m->env = env;
	i = 1;
	while (argv && argv[0] && argv[i])
	{
		if (!(new = ft_calloc(1, sizeof(t_env))))
			return (1);
		new->value = ft_strdup(argv[i]);
		new->next = NULL;
		env->next = new;
		env = new;
		i++;
	}
	return (0);
}

