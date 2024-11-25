/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spliter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:09:49 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/21 16:41:05 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_char_in_set(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	i;
	size_t	start;
	size_t	end;

	start = 0;
	while (s1[start] && ft_char_in_set(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_char_in_set(s1[end - 1], set))
		end--;
	str = (char *) malloc(sizeof(*s1) * (end - start + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (start < end)
		str[i++] = s1[start++];
	str[i] = '\0';
	return (str);
}


static size_t	count_words(char const *s, char c)
{
	size_t	words;
	size_t	i;
	bool	dq;
	bool	sq;
	bool	wbol;

	words = 0;
	i = 0;
	sq = 0;
	dq = 0;
	wbol = 0;
	while (s[i])
	{
		if (!sq && !dq)
		{
			if (s[i] != c && !wbol)
			{
				wbol = !wbol;
				words++;
			}
			if (s[i] == c)
				wbol = 0;
		}
		if (s[i] == '"' && !sq)
			dq = !dq;
		else if (s[i] == '\'' && !dq)
			sq = !sq;
		i++;
	}
	fprintf(stderr,"cwords: %d\n",words);
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


int	filler(char *src, int words, char **splitted, char c)
{
	char	**res;
	int		i;
	int		j;
	int		b;
	int		len;
	bool	dq;
	bool	sq;
	int		count;

	count = 0;
	i = 0;
	j = 0;
	b = 0;
	len = 0;
	dq = 0;
	sq = 0;
	while (src[i] && j < words)
	{
		if (!dq && !sq)
		{
			while (src[i + count])
			{
				if (src[i + count] == c && !sq && !dq)
					break ;
				if (src[i + count] == '"' && !sq)
					dq = !dq;
				else if (src[i + count] == '\'' && !dq)
					sq = !sq;
				count++;
			}
		}
		if (src[i + count] == '"' && !sq)
			dq = !dq;
		else if (src[i + count] == '\'' && !dq)
			sq = !sq;
		if (count)
		{
			splitted[j++] = ft_substr(src, i, count);
			i += count;
			count = 0;
		}
		else	
			i++;	
	}
	splitted[j] = 0;
	return (0);
}


char	**ft_split_quotes(char const *s, char c)
{
	size_t	words;
	char	**tab;
	int		a;

	words = count_words(s, c);
	tab = ft_calloc((words + 1), sizeof(char *));
	if (!tab)
		return (NULL);
	filler(s, words, tab, c);
	//print_tab(tab);
	return (tab);
}