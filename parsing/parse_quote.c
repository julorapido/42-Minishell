/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:32:38 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/25 12:22:06 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
static void	parse_quote3(int *i, char **s_cmds, char *s_quote)
{
	int	p;
	int	o;

	o = (*i);
	(*i)++;
	while (ft_strchr(s_cmds[(*i)], '\"') == NULL)
	{
		s_quote = fn_realloc_strcat(s_quote, s_cmds[(*i)], 1);
		(*i)++;
		if (!s_cmds[(*i)])
			break ;
	}
	if ((*i) != o + 1 && s_cmds[(*i)] != NULL)
	{
		s_quote = fn_realloc_strcat(s_quote, s_cmds[(*i)], 1);
		(*i)++;
	}
	p = (!s_cmds[(*i)]) ? ((*i) - 1) : (*i);
	if ((*i) != o + 1)
	{
		while (p >= o)
		{
			free(s_cmds[p]);
			p--;
		}
	}
}

static void	parse_quote2(token **cmd_tokens, char **s_cmds, int *i)
{
	int		quote_ix;
	char	*al_s2;
	char	*al_s3;

	al_s2 = ft_substr(s_cmds[(*i)], 1, ft_strlen(s_cmds[(*i)]) - 1);
	quote_ix = ft_m_strchr_i(al_s2, '\"', '\"') + 2;
	token_push(cmd_tokens, token_new(ft_substr(s_cmds[(*i)], 0, quote_ix), QUOTE));
	al_s3 = s_cmds[(*i)];
	s_cmds[(*i)] = ft_substr(s_cmds[(*i)], quote_ix, ft_strlen(s_cmds[(*i)]));
	free(al_s2);
	free(al_s3);
}

void	parse_quote(token **cmd_tokens, char **s_cmds, int *i)
{
	char	*s_quote;
	char	*al_s1;		

	s_quote = ft_calloc(ft_strlen(s_cmds[(*i)]) + 1, 1);
	if (!s_quote)
		return ;
	ft_strlcpy(s_quote, s_cmds[(*i)], ft_strlen(s_cmds[(*i)]) + 1);
	al_s1 = ft_substr(s_cmds[(*i)], 1, ft_strlen(s_cmds[(*i)]));
	if (s_cmds[(*i) + 1] && (s_cmds[(*i)][ft_strlen(s_cmds[(*i)]) - 1] != '\"')
		&& ft_strchr(al_s1, '\"') && ft_strchr(al_s1, ' '))
		parse_quote3(i, s_cmds, s_quote);
	else if (ft_strchr(al_s1, '\"') && (s_cmds[(*i)][ft_strlen(s_cmds[(*i)]) - 1] != '\"'))
	{
		free(s_quote);
		parse_quote2(cmd_tokens, s_cmds, i);
		return ;
	}
	else
		free(s_cmds[(*i)]);
	free(al_s1);
	token_push(cmd_tokens, token_new(s_quote, QUOTE));
	(*i)++;
}*/

void	parse_quote(token **cmd_tokens, char **s_cmds, int *i)
{
	char *s;

	s = ft_strdup(s_cmds[(*i)]);
	// start w quote
	if(*s == '\"')
	{
		// ex: $ echo "acbde"
		int ix = ft_m_strchr_i(ft_substr(s_cmds[*i], 1, ft_strlen(s_cmds[*i]) - 1), '\"', '\"');
		if (ix != -1)
		{
			s = ft_substr(s_cmds[(*i)], 0, ix + 2);
			token_push(cmd_tokens, token_new(s, QUOTE));
			if(ix + 1 != ft_strlen(s_cmds[(*i)]) - 1)
			{
				s_cmds[(*i)] = ft_substr(s_cmds[(*i)], ix + 2, ft_strlen(s_cmds[(*i)]));
			}else
				(*i)++;
			return ;
		}
		(*i)++;
		if (s_cmds[*i])
		{
			while (ft_m_strchr_i(s_cmds[*i], '\"', '\"') == -1)
			{
				s = fn_realloc_strcat(s, s_cmds[(*i)], 1);
				(*i)++;
			}
			if(s_cmds[*i])
			{
				// ex: $ echo "ab ce a"
				if (ft_m_strchr_i(s_cmds[*i], '\"', '\"') == ft_strlen(s_cmds[*i]) - 1)
				{
					s = fn_realloc_strcat(s, s_cmds[(*i)], 1);
					(*i)++;	
				}
				// ex: $ echo "ac f v"f e
				else
				{
					char *str = ft_substr(s_cmds[(*i)], 0, ft_m_strchr_i(s_cmds[*i], '\"', '\"') + 1);
					s = fn_realloc_strcat(s, str, 1);
					s_cmds[(*i)] = ft_substr(s_cmds[(*i)], 
						ft_m_strchr_i(s_cmds[*i], '\"', '\"') + 1, 
						ft_strlen(s_cmds[(*i)])
					);
				}
			}
		}
		token_push(cmd_tokens, token_new(s, QUOTE));
	}
	// doesnt start with quote ""
	else
	{
		// ex: $ echo ab"cd efg"
		int x = ft_m_strchr_i(s, '\"', '\"');
		char *left = ft_substr(s, 0, x);
		
		char *quoted_subs = ft_substr(s, x, ft_strlen(s));
		token_push(cmd_tokens, token_new(left, COMMAND));
		s_cmds[(*i)] = quoted_subs;
		return;
	}
}

