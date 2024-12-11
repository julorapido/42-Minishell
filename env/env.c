/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:45:02 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/11 17:25:13 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	size_t_var(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}

char	*gay_env(t_minishell *t_m, char *v)
{
	t_env	*n;
	char	*s;

	s = NULL;
	n = (t_m->env);
	while (n)
	{
		if(n->value)
		{
			///printf("N->VALUE: %s (size_t_var = %d) \n", n->value, size_t_var(n->value));
			if (ft_strncmp(n->value, v, size_t_var(n->value)) == 0)
			{
				s = ft_substr(n->value, size_t_var(n->value) + 1, FT(n->value));
				break ;
			}
		}
		n = n->next;
	}
	return (s);
}

int	env_init(t_minishell *t_m, char **argv)
{
	t_env	*env;
	t_env	*new;
	int		i;

	env = ft_calloc(1, sizeof(t_env));
	if (!(env))
		return (1);
	env->value = ft_strdup(argv[0]);
	env->next = NULL;
	t_m->env = env;
	i = 1;
	while (argv && argv[0] && argv[i])
	{
		new = ft_calloc(1, sizeof(t_env));
		if (!(new))
			return (1);
		new->value = ft_strdup(argv[i]);
		new->next = NULL;
		env->next = new;
		env = new;
		i++;
	}
	return (0);
}
