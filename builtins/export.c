/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:26:34 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/21 16:07:53 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_len(t_env *t)
{
	int		i;
	t_env	*h;

	i = 0;
	h = t;
	while(h)
	{
		i++;
		h = h->next;
	}
	return (i);
}

static int	left_part_len(t_env *t)
{
	int		i;

	i = 0;
	while(t->value[i] != '=')
		i++;
	return (i);
}

static void	display(int argc, char *argv[])
{
	int	i;

	i = 0;
	while (i++ < argc - 1)
	{
		printf("%s \n", argv[i]);	
	}
}


void	f__export(t_minishell *t)
{
	t_env	*h;
	char	**w;
	int		i;
	int		j;
	int		l;

	i = 0;
	h = t->env;
	l = env_len(t->env);
	w = (char **) malloc(env_len(t->env) * sizeof(char *));
	while (h)
	{
		w[i] = h->value;
		i++;
		h = h->next;
	}
	i = 0;	
	while (i < (l - 1))
	{
		if (i + 1 < l && ft_strcmp(w[i + 1], w[i]) < 0)
		{
			w[l + 1] = w[i];
			w[i] = w[i + 1];
			w[i + 1] = w[l + 1];
			i = 0;
		}
		i++;
	}
	display(env_len(t->env), w);	
}
