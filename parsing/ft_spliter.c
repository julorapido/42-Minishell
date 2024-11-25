/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spliter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:09:49 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/25 12:51:17 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (words);
}

static char *rm_quotes(char *s, char c)
{
	size_t	words;
	size_t	i;
	bool	dq;
	bool	sq;
	int		offset;

	words = 0;
	i = 0;
	sq = 0;
	dq = 0;
	offset = 0;
	while (s[i + offset])
	{
		if (s[i + offset] == '"' && !sq)
		{
			dq = !dq;
			offset++;
		}
		else if (s[i + offset] == '\'' && !dq)
		{
			sq = !sq;
			offset++;
		}
		if (sq != dq)
		{
			if (sq)
			{
				if (s[i + offset] == '\'')
					offset++;
			}
			else if (dq)
			{
				if (s[i + offset] == '"')
					offset++;
			}
		}
		s[i] = s[i + offset];
		i++;
	}
	s[i] = '\0';
	return (s);
}


static void	apply_quote_removal(char **t)
{
	size_t	i;

	i = 0;
	while (t[i])
	{
		t[i] =  rm_quotes(t[i], ' ');
		i++;
	}
	return ;
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



static int	filler(char *src, int words, char **splitted, char c)
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


char	**ft_split_quotes(char *s, char c, int remove_quote)
{
	size_t	words;
	char	**tab;
	int		a;

	words = count_words(s, c);
	tab = ft_calloc((words + 1), sizeof(char *));
	if (!tab)
		return (NULL);
	filler(s, words, tab, c);
	if(remove_quote)
		apply_quote_removal(tab);
	return (tab);
}

