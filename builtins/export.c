/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:26:34 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/06 18:03:42 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	env_lastadd(t_env *t, char *value)
{
	t_env	*h;
	t_env	*new;

	h = t;
	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		return ;
	while (h->next)
	{
		h = h->next;
	}
	h->next = new;
	new->value = ft_strdup(value);
	new->next = NULL;
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

static int	s_and_chang(char *arg, t_env *env)
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

static int	handle_e(char *s)
{
	int		i;
	char	*t;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	t = ft_substr(s, 0, i);
	if (FM(t, '-', '-') == -1)
		return (1);
	return (0);
}

int	f__export(t_minishell *t)
{
	int		i;
	int		last_ex;

	last_ex = (EXIT_SUCCESS);
	i = 1;
	while (t->c_args[i])
	{
		if ((t->c_args[i][0]) == '=')
			return (1);
		if ((FM(t->c_args[i], '=', '=') == -1 && !ft_str_isalpha(t->c_args[i]))
			|| (!handle_e(t->c_args[i])))
		{
			t->exstat = 1;
			fprintf(stderr, "condition ext %d\n", t->exstat);
			return (1);
		}
		last_ex = (0);
		s_and_chang(t->c_args[i], t->env);
		i++;
	}
	return (last_ex);
}
