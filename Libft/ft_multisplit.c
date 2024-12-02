/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multisplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:14:13 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/02 14:44:47 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdbool.h>

static size_t	count_words(char *s, char *set)
{
	size_t	words;
	size_t	i;
	char	q;

	q = '\0';
	words = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
			q = (!q) ? (s[i]) : ((s[i] == q) ? ('\0') : (q));
		if (((ft_inset(s[i], set) >= 0 && !q) && s[i + 1] != '\0'))
			words++;
		i++;
	}
	return (words+1);
}

int	fill_tab(t_mltsplit *new, char *s, char *set)
{
	size_t	i;
	char	q = '\0';

	i = 0;
	while (s[i] && (ft_inset(s[i], set) < 0 || q))
	{
		if (s[i] == '\'' || s[i] == '\"')
			q = (!q) ? (s[i]) : ((s[i] == q) ? ('\0') : (q));
		new->s[i] = s[i];
		i++;
	}
	new->s[i] = '\0';
	return (ft_inset(s[i], set));
}

static void	free_tab(t_mltsplit *t)
{
	size_t	i;

	i = 0;
	while (t[i].s)
	{
		if (t[i].s)
			free(t[i].s);
		free(t);
		i++;
	}
	free(t);
	return ;
}

static int	set_mem(t_mltsplit *tab, char *s, char *set)
{
	size_t	count;
	size_t	ix;
	size_t	i;
	int		is_skip = 0;
	char	q = '\0';

	ix = 0;
	i = 0;
	while (s[ix])
	{
		count = 0;
		while (s[ix + count] && (ft_inset(s[ix + count], set) < 0 || q))
		{
			if (s[ix + count] == '\'' || s[ix + count] == '\"')
				q = (!q) ? (s[ix + count]) : ((s[ix + count] == q) ? ('\0') : (q));
			count++;
		}
		if (count > 0 || (is_skip))
		{
			tab[i].s = malloc(sizeof(char) * (count + 1));
			if (!tab[i].s)
				return (-1);
			i++;
			tab[i].ix = fill_tab(&(tab[i]), (s + ix), set);
		}
		is_skip = 0;
		if (count == 0)
		{
			count = 1;
			is_skip = 1;
		}
		ix += (count);
	}
	tab[i].s = NULL;
	return (1);
}

t_mltsplit	*ft_multisplit(char *s, char *set)
{
	size_t		words;
	t_mltsplit	*tab;
	int			a;

	words = count_words(s, set);
	tab = malloc((words + 1) * sizeof(t_mltsplit));
	if (!tab)
		return (NULL);
	(tab[0]).mltsplit_l = (int)(words);
	a = set_mem(tab, s, set);
	if (a == -1)
	{
		free_tab(tab);
		return (NULL);
	}
	if (!ft_strlen(tab[0].s))
		tab[0].ix = ft_inset(*s, set);
	return (tab);
}
