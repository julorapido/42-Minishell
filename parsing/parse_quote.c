/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:32:38 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/25 17:54:40 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	parse_quote(t_minishell *t_m, token **cmd_tokens, char **s_cmds, int *i)
{
	char *s;

	s = ft_strdup(s_cmds[(*i)]);
	// start w quote
	if(*s == '\"')
	{
		bool ign_space = false;
		int ix = ft_m_strchr_i(ft_substr(s_cmds[*i], 1, ft_strlen(s_cmds[*i]) - 1), '\"', '\"');
		// ex: $ echo "acbde"
		if (ix == ft_strlen(s_cmds[(*i)]) - 2)
		{
			s = ft_substr(s_cmds[(*i)], 0, ix + 2);
			if(ix + 1 != ft_strlen(s_cmds[(*i)]) - 1)
			{
				s_cmds[(*i)] = ft_substr(s_cmds[(*i)], ix + 2, ft_strlen(s_cmds[(*i)]));
			}else
				(*i)++;
		}
		// ex: $ echo ab"c "de
		else
		{
			s = ft_substr(s_cmds[*i], 0, ix + 2);
			s_cmds[*i] = ft_substr(s_cmds[*i], ix + 2, ft_strlen(s_cmds[*i]));
			ign_space = true;
		}
		token *t = token_new(s, QUOTE);
		t->quote_ignoreSpace = (ign_space);
		token_push(cmd_tokens, t);
	}
	// doesnt start with quote ""
	else
	{
		// ex: $ echo ab"cd efg"
		int x = ft_m_strchr_i(s, '\"', '\"');
		char *left = ft_substr(s, 0, x);
		
		int len = ( ft_last_strchr_i(s, '\"', '\"') - x) + 1;
		token *t = token_new(ft_substr(s, x, len), QUOTE);
		t->quote_ignoreSpace = true;
		t_m->stocked_token = t;
		if(len != ft_strlen(s) - 1)
		{
			t_m->stocked_scmdsi = ft_substr(s, ft_last_strchr_i(s, '\"', '\"') + 1,
			ft_strlen(s) - ft_last_strchr_i(s, '\"', '\"'));
		}
		// token_push(cmd_tokens, t);
		s_cmds[(*i)] = left;
	}
}

