/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:21:49 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/25 14:21:49 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
 int	size_t_var(char *s)
{
	int i;

	i = 0;
	while(s[i] && s[i] != '=')
		i++;
	return (i);
}

void	free_node(t_minishell *t_m, t_env *env)
{
	if (t_m->env == env && env->next == NULL)
	{
		ft_memdel(t_m->env->value);
		t_m->env->value = NULL;
		t_m->env->next = NULL;
		return ;
	}
	ft_memdel(t_m->env->value);
	ft_memdel(t_m->env);
}

int	f__unset(t_minishell *t_m)
{
	t_env	*h;	
	char	**args;
	int		i;

	args = t_m->c_args;
	if (!args[1])
	{
		ft_putstr_fd("unset: not enough arguments\n", 2);
		return (ERROR);
	}
	h = t_m->env;
	while(h && h->next)
	{	
		i = 1;
		while (args[i])
		{
			if(ft_strncmp((h)->next->value, args[i], size_t_var((h)->next->value)) == 0)
			{
				fprintf(stderr, "success removing: %s\n", h->next->value);
				//tmp = (h)->next;
				// free_node(t_m, h->next);
				//h = tmp;
				h->next = h->next->next;
			}
			i++;
		}
		h = h->next;
	}
	return (SUCCESS);
}

