/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expands.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:44:15 by julessainth       #+#    #+#             */
/*   Updated: 2024/12/04 17:36:40 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*insert_replace(int a, int b, char *s, char *insert_s)
{
	char	*ir;
	int		i;
	int		j;

	ir = (char *)(malloc(FT(s) - (b) + (FT(insert_s)) + 1));
	i = 0;
	while (s[i] && i < a)
	{
		ir[i] = s[i];
		i++;
	}
	j = i;
	a = 0;
	while (insert_s[a])
	{
		ir[i] = insert_s[a];
		i++;
		a++;
	}
	while (s[j + b])
	{
		ir[i] = s[j + b];
		i++;
		j++;
	}
	return (ir[i] = '\0', ir);
}

static char	*insert_expands(char *s, t_minishell *t)
{
	int		i;
	int		j;
	char	*sub_s;
	bool	q;

	q = false;
	i = 0;
	while (s[i])
	{
		if(s[i] == '\'')
			q = s[i];
		if (s[i] == '$' && !q)
		{
			j = 1;
			while (s[i + j] != ' ' && s[i + j] != '\0' && s[i + j] != '$'
				&& s[i + j] != ';' && s[i + j] != '\"')
				j++;
			sub_s = ft_substr(s, i + 1, j - 1);
			if (sub_s[0] == '?')
				s = IR_(i, j, s, ft_itoa(t->exstat));
			else
			{
				if (findenv(sub_s, t->env))
				{
					if (FC_((findenv(sub_s, t->env))->value, '='))
						s = IR_(i, j, s, FC_((FE_(sub_s, t->env))->value, '=') + 1);
					else
						s = IR_(i, j, s, "");
				}
			}
			free(sub_s);
		}
		i++;
	}
	return (s);
}

void	apply_expands(t_minishell *t)
{
	int	e;
	int	f;

	e = 0;
	while ((size_t)(e) < t->cmd_count)
	{
		(&(t->cmds[e]))->command = insert_expands((&(t->cmds[e]))->command, t);
		f = 0;
		while (f < (&(t->cmds[e]))->f_i)
		{
			(&(t->cmds[e]))->files[f].f_name = insert_expands(
				(&(t->cmds[e]))->files[f].f_name, t);
			f++;
		}
		e++;
	}
}
