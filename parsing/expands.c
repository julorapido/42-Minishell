/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expands.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:44:15 by julessainth       #+#    #+#             */
/*   Updated: 2024/12/09 14:20:28 by jsaintho         ###   ########.fr       */
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


static char	*i_norm(char *s, int *i, int j, t_minishell *t)
{
	char	*sub_s;
	char	*stored_s;

	sub_s = ft_substr(s, *i + 1, j - 1);
	stored_s = s;
	if (sub_s[0] == '?')
	{
		s = FSF(FTS(s, 0, *i),
				FSF(ft_itoa(t->exstat), FTS(s, *i + 2, FT(s) - 1)));
	}
	else if (j != 1)
	{
		if (findenv(sub_s, t->env))
		{
			if (FC_((findenv(sub_s, t->env))->value, '='))
				s = IR_(*i, j, s, FC_((FE_(sub_s, t->env))->value, '=') + 1);
		}
		else
		{
			if(*i + j == (int)FT(s) && *i == 0)
				s = ft_strdup("");
			else
			{
				s = FSF(ft_substr(s, 0, *i), ft_substr(s, *i + j, FT(s)));
				(*i)--;
			}
		}
	}
	if (stored_s)
		free(stored_s);
	free(sub_s);
	return (s);
}

static char	*insert_expands(char *s, t_minishell *t)
{
	int		i;
	int		j;
	char	q;

	q = '\0';
	i = 0;
	if (!s || !ft_strlen(s))
		return (s);
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
			q = ft_inq(q, s[i]);
		if ((s[i] == '$' && s[i + 1]) && (!q || (q == '\"')))
		{
			j = 1;
			while (s[i + j] != ' ' && s[i + j] != '\0' && s[i + j] != '$'
				&& s[i + j] != ';' && s[i + j] != '\"' && s[i + j] != '\''
				&& s[i + j] != ':')
				j++;
			if(j > 1)
				s = i_norm(s, &i, j, t);
			if (ft_stronly(s, ' ') || !s[i])
				return (s);
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
