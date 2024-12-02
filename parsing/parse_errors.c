/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:13:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/02 15:10:54 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool triple_operator(char a, char b, char c)
{
    if (a == '>' || a == '<' || a == '|')
    {
        if (a == b && b == c)
            return (true);
    }
    if (a == '<' && b == '>' && c == '|')
        return (true);
    return (false);
}

static int  quote_errors(char *s_cmds)
{
	bool	in_q;
	bool	in_sq;
	int		i;

	in_q = false;
	in_sq = false;
	i = 0;
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
	return '\0';
}

static char cmd_verif(t_mltsplit *s, int i, int j)
{
    char    c;

    c = '\0';
    if (((s[i].s)[j] == '>' && (s[i].s)[j + 1] == '<')
        || ((s[i].s)[j] == '<' && (s[i].s)[j + 1] == '|')
        || ((s[i].s)[j] == '|' && (s[i].s)[j + 1] == '|')
        || ((s[i].s)[j] == '>' && (s[i].s)[j + 1] == '|')
        || ((s[i].s)[j] == '<' && (s[i].s)[j + 1] == '>')
    )
        c = ((s[i].s)[j + 1]);
    if ((s[i].s)[j + 1 + 1])
        if (triple_operator((s[i].s)[j], (s[i].s)[j + 1], (s[i].s)[j + 2]))
            c = ((s[i].s)[j]);
    return (c);
}

void    free_multisplit(t_mltsplit *s)
{
    int i;

    i = 0;
    while (i <= (*s).mltsplit_l)
    {
        if (s[i].s)
            free(s[i].s);
        i++;
    }
    free(s);
}

char parse_errors(t_mltsplit *s, char *line)
{
    int		i;
    int		j;
    char	c;

    c = '\0';
    i = 0;
    if (quote_errors(line) != '\0')
        return (quote_errors(line));
    while (s[i].s)
    {
        j = 0;
        while(s[i].s[j] != '\0')
        {
            if (!c && (s[i].s[j] == '\'' || s[i].s[j] == '\"'))
				c = s[i].s[j];
	        else if(c == s[i].s[j])
				c = '\0';
            if (s[i].s[j + 1] && !(c))
                if (cmd_verif(s, i, j) != '\0')
                    return (cmd_verif(s, i, j));
            j++;
        }
        i++;
    }
    if (s[i-1].s[ft_strlen(s[i-1].s) - 1] == '>' || s[i-1].s[ft_strlen(s[i-1].s) - 1] == '<')
        return (*(s[i - 1].s));
    return ('\0');
}
