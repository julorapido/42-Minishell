/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:46:03 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/27 12:18:00 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// PARSE [TOKENS]
static void	parse_tokens2(token **cmd_tokens, int *i, char **s_cmds, t_minishell *t_m)
{
	int		ix;
	char	*temp;
	char	*saved_s;

	ix = ft_m_strchr_i(s_cmds[(*i)], ';', '|');
	saved_s = ft_substr(s_cmds[(*i)], 0, ix);	
	if (ft_strlen(saved_s) > 0)
		token_push(cmd_tokens, token_new(saved_s, switcher(t_m, saved_s, cmd_tokens)));
	else
		free(saved_s);
	token_push(cmd_tokens, token_new("", M_TKEN( s_cmds[(*i)][ix] )));
	if ((size_t)(ix + 1) == ft_strlen(s_cmds[(*i)]))
	{
		free(s_cmds[(*i)]);
		(*i)++;
	}
	else
	{
		temp = s_cmds[(*i)];
		s_cmds[(*i)] = ft_substr(s_cmds[(*i)], ix + 1, ft_strlen(s_cmds[(*i)]));
		free(temp);
	}
}


int	parse_tokens(char *cmd, token **cmd_tokens, t_minishell *t_m)
{
	char	**s_cmds;
	int		i;

	i = 0;
	s_cmds = ft_split_quotes(cmd, ' ', 0);
	while (s_cmds[i] != NULL)
	{	
		if(ft_m_strchr_i(s_cmds[i], '\"', '\'') != -1)
			parse_quote(t_m, cmd_tokens, s_cmds, &i);
		else
		{
			if (ft_strlen(s_cmds[i]) == 1 || (!ft_strchr(s_cmds[i], ';') && !ft_strchr(s_cmds[i], '|'))) 
			{	
				if (ft_strcmp(s_cmds[i], ">>") == 0)
					token_dbl_push(t_m->cmd_tokens);
				else
				{
					if (strlen(s_cmds[i]) >= 2 && (ft_strchr(s_cmds[i], '>') || ft_strchr(s_cmds[i], '<') || ft_strchr(s_cmds[i], '|')))
						switcher(t_m, s_cmds[i], t_m->cmd_tokens);
					else
						token_push(t_m->cmd_tokens, token_new(s_cmds[i], switcher(t_m, s_cmds[i], t_m->cmd_tokens)));
				}
				if(t_m->stocked_token)
				{
					token_push(cmd_tokens, t_m->stocked_token);
					t_m->stocked_token = NULL;	
				}
				if(t_m->stocked_scmdsi)
				{
					s_cmds[i] = t_m->stocked_scmdsi;
					t_m->stocked_scmdsi = NULL;
				}
				else
					i++;
			}
			else
				parse_tokens2(t_m->cmd_tokens, &i, s_cmds, t_m);
		}
	}
	if(s_cmds[i])
		free(s_cmds[i]);
	free(s_cmds);
	return(0);
}
