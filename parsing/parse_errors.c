/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:13:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/06 11:48:53 by jsaintho         ###   ########.fr       */
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

	if (*line == '|')
		return ('|');
	i = 1;
	s = ft_multisplit(line, "<|>");
	while (s[i].s)
	{
		j = 0;
		while (s[i].s[j] && s[i].s[j] == ' ')
			j++;
		if ((size_t)j == FT(s[i].s))
		{
			c = "<|>"[s[i].ix];
			return (ft_free_multisplit(s), c);
		}
		i++;
	}
	return ('\0');
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

static char	cmd_verif(char **s, int i, int j)
{
	char	c;

	c = '\0';
	if (((s[i])[j] == '>' && (s[i])[j + 1] == '<')
		|| ((s[i])[j] == '<' && (s[i])[j + 1] == '|')
		|| ((s[i])[j] == '|' && (s[i])[j + 1] == '|')
		|| ((s[i])[j] == '>' && (s[i])[j + 1] == '|')
		|| ((s[i])[j] == '<' && (s[i])[j + 1] == '>')
	)
		c = ((s[i])[j]);
	if ((s[i])[j + 1 + 1])
		if (triple_operator((s[i])[j], (s[i])[j + 1], (s[i])[j + 2]))
			c = ((s[i])[j]);
	return (c);
}

char	parse_errors(char **s, char *line)
{
	int		i;
	int		j;
	char	c;

	if (quote_errors(line, &i, &c) != '\0')
		return (quote_errors(line, &i, &c));
	while (s[i])
	{
		j = 0;
		while (s[i][j] != '\0')
		{
			if (!c && (s[i][j] == '\'' || s[i][j] == '\"'))
				c = s[i][j];
			else if (c == s[i][j])
				c = '\0';
			if (s[i][j + 1] && !(c))
				if (cmd_verif(s, i, j) != '\0')
					return (cmd_verif(s, i, j));
			j++;
		}
		i++;
	}
	if ((i > 0 && j > 0) && (P_E == '>' || P_E == '<' || P_E == '|'))
		return (*(s[i - 1]));
	return ('\0');
}
