/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:21:49 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/11 11:19:15 by jsaintho         ###   ########.fr       */
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

static void	free_node(t_minishell *t_m, t_env *env)
{
	if (t_m->env == env && env->next == NULL)
	{
		ft_memdel(t_m->env->value);
		t_m->env->value = NULL;
		t_m->env->next = NULL;
		return ;
	}
	ft_memdel(env->value);
	ft_memdel(env);
}

int	f__unset(t_minishell *t_m)
{
	t_env	*h;
	t_env	*tmp;
	char	**args;
	int		i;
	int 	j;

	j = 2;
	args = t_m->c_args;
	if (!args[1])
	{
		ft_putstr_fd("unset: not enough arguments\n", 2);
		return (ERROR);
	}
	h = t_m->env;
	while(h && h->next)
	{
		//if (!h->value)
		//	continue;
		i = 1;
		//printf("%s : %d\n", h->value,  size_t_var((h->next)->value));
		while (args[i])
		{
			if(ft_strncmp((h->next)->value, args[i], size_t_var((h->next)->value)) == 0)
			{
				tmp = h->next->next;	
				free_node(t_m, h->next);
				h->next = tmp;
				j--;
			}
			i++;
		}
		h = h->next;
	}
	if (j > 0)
		return (ERROR);
	else
		return (SUCCESS);
}
