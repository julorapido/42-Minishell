/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:13:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/11 15:47:09 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	triple_operator(char a, char b, char c)
{
	void	*d;

	(void)d;
	if (a == '>' || a == '<' || a == '|')
	{
		if (a == b && b == c)
			return (true);
	}
	if (a == '<' && b == '>' && c == '|')
		return (true);
	return (false);
}

static char	empty_spaces(char *line)
{
	t_mltsplit	*s;
	int			i;
	int			j;
	char		c;

	if (!line || !FT(line))
		return ('|');
	i = 1;
	s = ft_multisplit(line, "<>");
	if(ft_strlen(s[0].s) && ft_stronly(s[0].s, ' ') && ft_char_in_set(*line, "<>"))
		return (ft_free_multisplit(s), *line);
	if (!s[i].s)
		return (ft_free_multisplit(s), '\0');
	while (s[i].s)
	{
		j = 0;
		while (s[i].s[j] && s[i].s[j] == ' ')
			j++;
		if ((size_t)j == FT(s[i].s) && (j > 0))
		{
			c = "<>"[s[i].ix];
			return (ft_free_multisplit(s), c);
		}
		i++;
	}
	return (ft_free_multisplit(s), '\0');
}

static int	quote_errors(char *s_cmds, int *p, char *c)
{
	bool	in_q;
	bool	in_sq;
	int		i;

	*p = 0;
	*c = '\0';
	in_q = false;
	in_sq = false;
	i = 0;
	if (empty_spaces(s_cmds) != '\0')
		return (empty_spaces(s_cmds));
	while (i < (int)ft_strlen(s_cmds))
	{
		if (s_cmds[i] == '\"' && (!in_sq))
			in_q = !(in_q);
		if (s_cmds[i] == '\'' && (!in_q))
			in_sq = !(in_sq);
		i++;
	}
	if (in_q)
		return ('\"');
	if (in_sq)
		return ('\'');
	return ('\0');
}

static char	cmd_verif(char *s, int i)
{
	char	c;

	c = '\0';
	if ((s[i] == '>' && s[i+1] == '<')
		|| (s[i] == '<' && s[i+1] == '|')
		|| (s[i] == '|' && s[i+1] == '|')
		|| (s[i] == '>' && s[i+1] == '|')
		|| (s[i] == '<' && s[i+1] == '>')
	)
		c = ((s[i]));
	if (s[i + 2])
		if (triple_operator(s[i], s[i+1], s[i + 2]))
			c = ((s[i]));
	return (c);
}

char	parse_errors(char *line)
{
	int		i;
	char	c;
	char	*s;
	char	*l;
	char	x;

	l = ft_strdup(line);
	if (quote_errors(l, &i, &c) != '\0')
	{
		x = (quote_errors(l, &i, &c));
		free(l);
		return (x);
	}
	s = ft_str_remvchr(l, ' ', ' ');
	while (s[i])
	{
		if (!c && (s[i] == '\'' || s[i] == '\"'))
			c = s[i];
		else if (c == s[i])
			c = '\0';
		if (s[i+1] && !(c))
		{
			if (cmd_verif(s, i) != '\0')
			{
				x = (cmd_verif(s, i));
				free(s);
				return (x);
			}
		}
		i++;
	}
	if ((i > 0) && (s[i-1] == '>' || s[i-1] == '<' || s[i-1] == '|'))
	{
		x = (s[i - 1]);
		free(s);
		return (x);
	}
	return (free(s), '\0');
}
