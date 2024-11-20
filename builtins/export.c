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
void printlist(char **list, char *name)
{
	int i;

	i = 0;
	if (!name)
		name = "(null)";
	while(list[i])
	{
		fprintf(stderr, "%s[%d]: %s\n", name, i, list[i]);
		i++;
	}
}

static int	env_len(t_env *t)
{
	int		i;
	t_env	*h;

	i = 0;
	h = t;
	while(h->next)
	{
		i++;
		h = h->next;
		//fprintf(stderr, "h-value: %s\n", h->value);
	}
	//fprintf(stderr, "i= %d\n", i+1);
	return (i+1);
}

// static int	left_part_len(t_env *t)
// {
// 	int		i;

// 	i = 0;
// 	while(t->value[i] != '=')
// 		i++;
// 	return (i);
// }

static void	display(int argc, char *argv[])
{
	int	i;

	i = 0;
	while (i++ < argc - 1)
	{
		printf("%s \n", argv[i]);	
	}
}

static int	env_lastadd(t_env *t, char *value)
{
	int		i;
	t_env	*h;
	t_env	*new;

	i = 0;
	h = t;
	new = ft_calloc(1, sizeof(t_env));
	if(!new)
		return (-1);
	while(h->next)
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
	while(h)
	{
		if (!ft_strncmp(name, h->value, ft_strlen(name)))
			return (h);
		h = h->next;
	}
	return (NULL);
}

int	s_and_chang(char *arg, t_env *env)
{
	char	 **tmp;
	t_env	*target;

	tmp = ft_split(arg, '=');
	target = findenv(tmp[0], env);
	if (target)
		target->value = ft_strdup(arg);
	else
		env_lastadd(env, arg);
	ft_free_tab
	 (tmp);
	return (0);
}

void	f__export(t_minishell *t, int fdout)
{
	t_env	*h;
	char	**w;
	int		i = fdout;
	int		l;
	char	*temp;

	i = 1;
	h = t->env;
	while(t->c_args[i] != 0)
	{	
		s_and_chang(t->c_args[i], h);
		i++;
	}
	h= t->env;
	i = 0;
	if (t->env)
	{
		l = env_len(t->env);
		//printlist(t->c_args, "c_args");
		w = ft_calloc(env_len(t->env) + 1, sizeof(char *));
		if(t->c_args[1] == 0)
		{
			while (h)
			{
				w[i] = h->value;
				i++;
				h = h->next;
			}
			i = 0;	
			while (i < (l-1))
			{
				if (ft_strcmp(w[i + 1], w[i]) < 0)
				{
					temp = w[i];
					w[i] = w[i + 1];
					w[i + 1] = temp;
					i = 0;
					continue ;
				}
				i++;
			}
			display(env_len(t->env), w);
		}
		free(w);
	}
}
