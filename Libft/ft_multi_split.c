/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multi_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:37:01 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/01 12:50:35 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strchr_i(const char *string, int searchedChar )
{
	char	*str;
	int		i_;

	i_ = 0;
	str = (char *)string;
	while (*str != searchedChar && *str != 0)
	{
		str++;
		i_++;
	}
	if (*str == searchedChar)
		return (i_);
	else
		return (-1);
}


static size_t	count_words(char const *s, char *set)
{
	size_t	words;
	size_t	i;

	words = 0;
	i = 0;
	while (s[i])
	{
		if ( (ft_strchr_i(set, s[i]) < 0)  && ( ft_strchr_i(set, s[i + 1]) > 0 || s[i + 1] == '\0') )
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

static int	set_mem(char **tab, char const *s, char *set)
{
	size_t	count;
	size_t	index;
	size_t	i;

	index = 0;
	i = 0;
	while (s[index])
	{
		count = 0;
		while (s[index + count] && ft_strchr_i(set, s[index + count]) > 0)
			count++;
		if (count > 0)
		{
			tab[i] = malloc(sizeof(char) * (count + 1));
			if (!tab[i])
				return (-1);
			fill_tab(tab[i], (s + index), set[ft_strchr_i(set, s[index + count])]);
			i++;
			index += count;
		}
		else
			index++;
	}
	tab[i] = 0;
	return (1);
}

char	**ft_multi_split(char const *s, char *set)
{
	size_t	words;
	char	**tab;
	int		a;

	words = count_words(s, set);
	tab = malloc(sizeof(char *) * (words + 1));
	if (!tab)
		return (NULL);
	a = set_mem(tab, s, set);
	if (a == -1)
	{
		free_tab(tab);
		return (NULL);
	}
	return (tab);
}
