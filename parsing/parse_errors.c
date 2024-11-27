/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:16:41 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/27 12:40:16 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	quote_errors(char	*s_cmds, t_minishell *t_m)
{
	bool	b;
	bool	in_q;
	bool	in_sq;
	int		i;
	int		q;
	int		q_s;

	q = 0;
	q_s = 0;
	b = false;
	in_q = false;
	in_sq = false;
	i = 0;
	while (i < ft_strlen(s_cmds))
	{
		if (s_cmds[i] == '\"' && (!in_sq))
			in_q = !(in_q);
	
		if (s_cmds[i] == '\'' && (!in_q))
			in_sq = !(in_sq);
		i++;
	}
	// printf("S_CMDS SINGLE[%c]{%d}   double[%c]{%d}\n", '\'', in_sq?1:0, '\"', in_q?1:0 );
	if (in_q)
		return (t_m->e_v[0] = '\"', t_m->parse_error = true, true);
	if (in_sq)
		return (t_m->e_v[0] = '\'', t_m->parse_error = true, true);
	return false;
}

int	parse_errors(char *cmd, t_minishell *t_m)
{
	char	*s_cmds;	
	int		i;

	t_m->parse_error = false;	
	t_m->splt_cat = ft_splitcat(ft_split(cmd, ' '));
	s_cmds = t_m->splt_cat;
	if(ft_strlen(s_cmds) == 0 || quote_errors(s_cmds, t_m))
		return (0);
	i = 0;
	if ((is_char_operator(s_cmds[0]) && ft_strlen(s_cmds) == 1) || s_cmds[0] == '|')
		return (t_m->e_v[0] = '/', t_m->e_v[1] = 'n', t_m->parse_error = true, 0);
	while (i < ft_strlen(s_cmds))
	{
		if (i + 1 < ft_strlen(s_cmds))
		{
			// > ;
			if (s_cmds[i] == '>' && s_cmds[i + 1] == ';')
				 t_m->parse_error = true;
			// triple >>> ;;; <<< /// """
			if (i + 1 + 1 < ft_strlen(s_cmds))
				if (is_char_operator(s_cmds[i]))
					if (s_cmds[i] == s_cmds[i + 1] && s_cmds[i + 1] == s_cmds[i + 2])
						t_m->parse_error = true;
			// >| <; |>
			if (is_char_operator(s_cmds[i + 1]) && is_char_operator(s_cmds[i]))
				if (s_cmds[i + 1] != s_cmds[i]
					&& !(s_cmds[i + 1] == '|' || s_cmds[i + 1] == ';') 
					&& !(s_cmds[i] == '.' || s_cmds[i] == '/' || s_cmds[i] == '\"')
					&& !(s_cmds[i] == '\"' && s_cmds[i + 1] == '\'')
					&& !(s_cmds[i] == '\'' && s_cmds[i + 1] == '\"')
				)
					t_m->parse_error = true;
			if (t_m->parse_error == true)
				return (t_m->e_v[0] = s_cmds[i], t_m->e_v[1] = s_cmds[i + 1], t_m->parse_error = true, 0);
		}
		i++;
	}	
	if (i > 0)
		if (s_cmds[i - 1] == '|' || s_cmds[i - 1] == '>' || s_cmds[i - 1] == '<')
			return (t_m->e_v[0] = s_cmds[i - 1], t_m->e_v[1] = 0, t_m->parse_error = true, 0);
	return (0);
}
