/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:13:14 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/29 16:56:55 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static  bool triple_operator(char a, char b, char c)
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

int  quote_errors(char	*s_cmds)
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
		return (1);
	if (in_sq)
		return (0);
	return -1;
}


char parse_errors(t_mltsplit *s)
{
    int     i;
    int     j;
    bool    in_q;

    in_q = false;
    i = 0;
    while (s[i].s)
    {
        j = 0;
        while(s[i].s[j] != '\0')
        {
            if(s[i].s[j] == '\'' || s[i].s[j] == '\"')
                in_q = !(in_q);
            if(s[i].s[j + 1] && !(in_q))
            {
                if(((s[i].s)[j] == '>' && (s[i].s)[j + 1] == '<')
                    || ((s[i].s)[j] == '<' && (s[i].s)[j + 1] == '|')
                    || ((s[i].s)[j] == '|' && (s[i].s)[j + 1] == '|')
                )
                    return ((s[i].s)[j + 1]);
                if((s[i].s)[j + 1 + 1])
                    if(triple_operator((s[i].s)[j], (s[i].s)[j + 1], (s[i].s)[j + 2]))
                        return((s[i].s)[j]);
            }
            j++;
        }
        i++;
    }
    if(s[i-1].s[ft_strlen(s[i-1].s) - 1] == '>' || s[i-1].s[ft_strlen(s[i-1].s) - 1] == '<')
        return (*(s[i - 1].s));
    return ('\0');
}