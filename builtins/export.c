/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchauvot <gchauvot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:26:34 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/07 11:23:04 by gchauvot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_lastadd(t_env *t, char *value)
{
	int		i;
	t_env	*h;
	t_env	*new;

	i = 0;
	h = t;
	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		return (-1);
	while (h->next)
	{
		i++;
		h = h->next;
	}
	h->next = new;
	new->value = ft_strdup(value);
	new->next = NULL;
	return (i);
}

t_env	*findenv(char *name, t_env *env)
{
	t_env	*h;

	h = env;
	while (h)
	{
		if (!ft_strncmp(name, h->value, ft_strlen(name))
			&& h->value[ft_strlen(name)] == '=')
			return (h);
		h = h->next;
	}
	return (NULL);
}

int	s_and_chang(char *arg, t_env *env)
{
	char	**tmp;
	t_env	*target;

	tmp = ft_split(arg, '=');
	target = findenv(tmp[0], env);
	if (target)
		target->value = ft_strdup(arg);
	else
		env_lastadd(env, arg);
	ft_free_tab(tmp);
	return (0);
}

void	f__export(t_minishell *t, int fdout)
{
	t_env	*h;
	int		i;

	i = 1;
	h = t->env;
	while (t->c_args[i] != 0)
	{
		s_and_chang(t->c_args[i], h);
		i++;
	}
	h = t->env;
}
