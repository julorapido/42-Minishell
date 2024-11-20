/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchauvot <gchauvot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:21:49 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/04 15:58:45 by gchauvot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
 int	size_t_var(char *s, char *d)
{
	int	i;
	int	j;

	i = 0;
	while(s[i] && s[i] != '=')
		i++;
	j = ft_strlen(d);
	if (j > i)
		return (j);
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

int arg_check(char *arg)
{
	int	i;

	i = 0;
	if(ft_isdigit(arg[0]))
		return (0);
	while (arg[i] != 0)
	{
		if (ft_isalnum(arg[i]) == 0 && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	unsetter1(t_env *h, char *args)
{
	t_env *tmp;

	while (h->next && h->next->next)
	{
		if(!ft_strncmp(h->next->value, args, size_t_var(h->next->value, args)))
		{
			tmp = h->next;
			if (h->next->next)
				h->next = h->next->next;
			else
				h->next = NULL;
			
			free(tmp->value);
			free(tmp);
		}
		h = h->next;
	}
	if(!ft_strncmp(h->next->value, args, size_t_var(h->next->value, args)))
	{
		free(h->next->value);
		free(h->next);
		h->next = NULL;
	}
	return (0);
}

int	unsetter(t_env *h, char *args, t_minishell *t_m)
{
	t_env *tmp;

	if(!ft_strncmp(h->value, args, size_t_var(h->value, args)))
	{
		t_m->env = h->next;
		tmp = h;
		h = h->next;
		free(tmp->value);
		free(tmp);
		return (-1);
	}
	unsetter1(h, args);
	return (0);
}


int	f__unset(t_minishell *t_m)
{
	t_env	*h;
	char	**args;
	int		i;

	args = t_m->c_args;
	if (!args[1])
		return (ft_putstr_fd("unset: not enough arguments\n", 2), ERROR);	
	h = t_m->env;
	i = 1;
	while (args[i])
	{
		if(!arg_check(args[i]))
		{
			ft_putstr_fd("unset: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd(": invalid parameter name\n", 2);
			i++;
			continue ;
		}
		unsetter(h, args[i], t_m);			
		h = t_m->env;
		i++;
	}
	return (SUCCESS);
}

