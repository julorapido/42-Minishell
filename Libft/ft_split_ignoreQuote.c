/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_ignoreQuote.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:20:13 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/21 16:30:59 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdbool.h>

static size_t	count_words(char const *s, char c)
{
	size_t	words;
	size_t	i;
	bool	is_q = false;

	words = 0;
	i = 0;
	while (s[i])
	{
		if(s[i] == '\"')
			is_q = (!is_q);
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0') && !is_q)
			words++;
		i++;
	}
	return (words);
}

static void	fill_tab(char *new, char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
}

static void	free_tab(char **t)
{
	size_t	i;

	i = 0;
	while (t[i])
	{
		free(t[i++]);
	}
	free(t);
	return ;
}

static int	set_mem(char **tab, char const *s, char c)
{
	size_t	count;
	size_t	ix;
	size_t	i;

	ix = 0;
	i = 0;
	while (s[ix])
	{
		count = 0;			
		while (s[ix + count] && s[ix + count] != c)
			count++;
		if (count > 0)
		{
			printf("SLICE IS [%zu, %d] [%c, %c] \n" , ix, c, s[ix], s[count]);
			tab[i] = malloc(sizeof(char) * (count + 1));
			if (!tab[i])
				return (-1);
			fill_tab(tab[i], (s + ix), c);
			i++;
			ix += count;
		}
		else
			ix++;
	}
	tab[i] = 0;
	return (1);
}

char	**ft_split_ignoreQuote(char const *s, char c)
{
	size_t	words;
	char	**tab;
	int		a;

	words = count_words(s, c);
	printf("SPIT on {%s} IGN_QUOTE %zu \n", s, words);
	tab = ft_calloc((words + 1),sizeof(char *));
	if (!tab)
		return (NULL);
	a = set_mem(tab, s, c);
	if (a == -1)
	{
		free_tab(tab);
		return (NULL);
	}
	return (tab);
}
