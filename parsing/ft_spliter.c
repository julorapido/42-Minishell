/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spliter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:09:49 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/25 16:51:32 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(char src, bool *dq, bool *sq)
{
	if (src == '"' && !(*sq))
		*dq = !(*dq);
	else if (src== '\'' && !(*dq))
		*sq = !(*sq);
	return (0);
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
		if (!sq && !dq && s[i] != c && !wbol)
		{
			wbol = !wbol;
			words++;
		}
		if (!sq && !dq && s[i] == c)
			wbol = 0;
		check_quotes(s[i], &dq, &sq);
		i++;
	}
	return (words);
}

char *rm_quotes(char *s)
{
	size_t	i;
	bool	dq;
	bool	sq;
	int j;

	i = 0;
	j = 0;
	sq = 0;
	dq = 0;
	while (s[i])
	{
		if (s[i] == '"' && !sq)
			dq = !dq;
		else if (s[i] == '\'' && !dq)
			sq = !sq;
		else
			s[j++] = s[i];
		i++;
	}
	s[j] = '\0';
	return s;
}

void	print_tab(char **t)
{
	size_t	i;

	i = 0;
	while (t[i])
	{
		// // t[i] = ft_strtrim(t[i], "\'\"");
		// fprintf(stderr, "b4 rm_tab[%d]: &%s&\n",i,t[i]);
		t[i] =  rm_quotes(t[i]);
		//fprintf(stderr, "post rm tab[%ld]: &%s&\n",i,t[i]);
		i++;
	}
	return ;
}

int	setres(char *src, int *i, bool *dq, bool *sq, char c)
{
	int count;

	count = 0;
	while (src[*i + count])
	{
		if (src[*i + count] == c && !(*sq) && !(*dq))
			break ;
		check_quotes(src[*i + count], dq, sq);
		count++;
	}
	return (count);
}

int	filler(char *src, int words, char **splitted, char c)
{
	int		i;
	int		j;
	bool	dq;
	bool	sq;
	int		count;

	i = 0;
	j = 0;
	dq = 0;
	sq = 0;
	while (src[i] && j < words)
	{
		if (!dq && !sq)
			count = setres(src, &i, &dq, &sq, c);
		if (count)
			splitted[j++] = ft_substr(src, i, count);
		i += count ? count : 1;
	}
	splitted[j] = 0;
	return (0);
}

static void	apply_quote_removal(char **t)
{
	size_t	i;

	i = 0;
	while (t[i])
	{
		t[i] =  rm_quotes(t[i]);
		i++;
	}
	return ;
}

char	**ft_split_quotes(char *s, char c, int remove_quote)
{
	size_t	words;
	char	**tab;
	
	words = count_words(s, c);
	tab = ft_calloc((words + 1), sizeof(char *));
	if (!tab)
		return (NULL);
	filler(s, words, tab, c);
	if(remove_quote)
		apply_quote_removal(tab);
	return (tab);
}