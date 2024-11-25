/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:46:03 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/25 14:11:35 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	parse_errors(char *cmd, t_minishell *t_m)
{
	char	*s_cmds;	
	int		i;
	int		q;
	int		q_s;
	int		l;

	t_m->parse_error = false;	
	t_m->splt_cat = ft_splitcat(ft_split(cmd, ' '));
	s_cmds = t_m->splt_cat;
	l = ft_strlen(s_cmds);
	if(l == 0)
		return (0);
	i = 0;
	q = q_s = 0;
	if ((is_char_operator(s_cmds[0]) && ft_strlen(s_cmds) == 1) || s_cmds[0] == '|')
		return (t_m->e_v[0] = '/', t_m->e_v[1] = 'n', t_m->parse_error = true, 0);
	while (i < l)
	{
		if (s_cmds[i] == '\"')
			q++;
		if (s_cmds[i] == '\'')
			q_s++;
		if (i + 1 < l)
		{
			// > ;
			if (s_cmds[i] == '>' && s_cmds[i + 1] == ';')
				 t_m->parse_error = true;
			// triple >>> ;;; <<< /// """
			if (i + 1 + 1 < l)
				if (is_char_operator(s_cmds[i]))
					if (s_cmds[i] == s_cmds[i + 1] && s_cmds[i + 1] == s_cmds[i + 2])
						t_m->parse_error = true;
			// >| <; |>
			if (is_char_operator(s_cmds[i + 1]) && is_char_operator(s_cmds[i]))
				if (s_cmds[i + 1] != s_cmds[i]
					&& !(s_cmds[i + 1] == '|' || s_cmds[i + 1] == ';') 
					&& !(s_cmds[i] == '.' || s_cmds[i] == '/' || s_cmds[i] == '\"')
				)
					t_m->parse_error = true;
			if (t_m->parse_error == true)
				return (t_m->e_v[0] = s_cmds[i], t_m->e_v[1] = s_cmds[i + 1], t_m->parse_error = true, 0);
		}
		i++;
	}	
	if (q % 2 != 0)
		return (t_m->e_v[0] = '\"', t_m->parse_error = true, 0);
	if (q_s % 2 != 0)
		return (t_m->e_v[0] = '\'', t_m->parse_error = true, 0);
	if (i > 0)
		if (s_cmds[i - 1] == '|' || s_cmds[i - 1] == '>' || s_cmds[i - 1] == '<')
			return (t_m->e_v[0] = s_cmds[i - 1], t_m->e_v[1] = 0, t_m->parse_error = true, 0);
	return (0);
}

// PARSE [TOKENS]
static void	parse_tokens2(token **cmd_tokens, int *i, char **s_cmds)
{
	int		ix;
	char	*temp;
	char	*saved_s;

	ix = ft_m_strchr_i(s_cmds[(*i)], ';', '|');
	saved_s = ft_substr(s_cmds[(*i)], 0, ix);	
	if (ft_strlen(saved_s) > 0)
		token_push(cmd_tokens, token_new(saved_s, switcher(saved_s, cmd_tokens)));
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
		printf("PARSE S_CMDS[%d]: %s \n", i, s_cmds[i]);
		if(ft_m_strchr_i(s_cmds[i], '\"', '\"') != -1)
			parse_quote(cmd_tokens, s_cmds, &i);
		else
		{
			if (ft_strlen(s_cmds[i]) == 1 || (!ft_strchr(s_cmds[i], ';') && !ft_strchr(s_cmds[i], '|'))) 
			{	
				if (ft_strcmp(s_cmds[i], ">>") == 0)
					token_dbl_push(t_m->cmd_tokens);
				else
				{
					if (strlen(s_cmds[i]) >= 2 && (ft_strchr(s_cmds[i], '>') || ft_strchr(s_cmds[i], '<') || ft_strchr(s_cmds[i], '|')))
						switcher(s_cmds[i], t_m->cmd_tokens);
					else
						token_push(t_m->cmd_tokens, token_new(s_cmds[i], switcher(s_cmds[i], t_m->cmd_tokens)));
				}
				i++;
			}
			else
				parse_tokens2(t_m->cmd_tokens, &i, s_cmds);
		}
	}
	if(s_cmds[i])
		free(s_cmds[i]);
	free(s_cmds);
	return(0);
}
