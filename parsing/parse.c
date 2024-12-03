/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:05:20 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/03 08:44:09 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
static void p_commands(t_cmd *t, int l)
{
    for(int i = 0; i < l; i++)
    {
        printf("-> %s ", t[i].command);
        printf("[");
        for(int j = 0; j < t[i].f_i; j++)
            if(!t[i].files[j]._out)
                printf("%s: %s ", 
                    t[i].files[j].heredoc ? "heredoc" : "in",
                    t[i].files[j].f_name
                );
        printf("] [");
        for(int j = 0; j < t[i].f_i; j++)
            if(t[i].files[j]._out)
                printf("%s: %s ",
                    t[i].files[j].append ? ">>" : "out", 
                    t[i].files[j].f_name
                );
        printf("]\n");
    }
}
*/

static char	*handle_spaces(char *str_token, t_cmd *ct)
{
	t_mltsplit	*s;
	int			i;
	int			c;
	char		*a;

	c = 0;
	i = 0;
	s = ft_multisplit(str_token, " ");
	while (s[c].s && ft_strlen(s[c].s) == 0)
		c++;
	while (s[i].s)
		i++;
	if (i == 0)
		return (ft_strdup(str_token));
	i = c + 1;
	while (s[i].s)
	{
		if (ft_strlen(s[i].s) == 0)
			s[i].s = ft_strdup(" ");
		ct->command = FSF(ct->command, FSF(FSR(" "), FSR(s[i].s)));
		i++;
	}
	return (a = ft_strdup(s[c].s), free_multisplit(s), a);
}

static void	fdp_parsing2(t_minishell *t, t_mltsplit *s, int *i)
{
	int	a;

	t->sq = ft_multisplit(s[*i].s, t->set);
	t->T = (*s[*i].s) == '>' || (*s[*i].s == '<');
	a = !(t->T);
	t->cmds[*i].command = ft_strdup(M_1(t->T, (t->sq[0]).s));
	while ((t->sq[a]).s)
	{
		if (t->set[(t->sq[a]).ix] == '<' && (a - 1 >= 0))
			if (t->set[(t->sq[a - 1]).ix] == '<')
				t->cmds[*i].files[t->cmds[*i].f_i].heredoc = true;
		if (t->set[(t->sq[a]).ix] == '>' && FT(t->sq[a].s) && (a - 1 >= 0))
			if ((!FT(t->sq[a - 1].s)) && (t->set[(t->sq[a - 1]).ix] == '>'))
				t->cmds[*i].files[t->cmds[*i].f_i].append = true;
		if (ft_strlen(t->sq[a].s))
		{
			TO[*i].files[TO[*i].f_i].f_name = HS((t->sq[a]).s, &(TO[*i]));
			TO[*i].files[TO[*i].f_i]._out = (!(t->set[(t->sq[a]).ix] == '<'));
			t->cmds[*i].n_out += (t->set[(t->sq[a]).ix] == '>');
			t->cmds[*i].n_in += (t->set[(t->sq[a]).ix] == '<');
			t->cmds[*i].f_i++;
		}
		a++;
	}
	free_multisplit(t->sq);
}

void	fdp_parsing(char *cmd, t_minishell *t)
{
	int			i;
	t_mltsplit	*s;

	s = ft_multisplit(cmd, "|");
	t->cmds = (t_cmd *) malloc((*s).mltsplit_l * sizeof(t_cmd));
	t->cmd_count = (*s).mltsplit_l;
	i = 0;
	while (s[i].s)
	{
		if (ft_m_strchr_i(s[i].s, '>', '<') != -1)
			fdp_parsing2(t, s, &i);
		else
			t->cmds[i].command = ft_strdup(s[i].s);
		i++;
	}
	free_multisplit(s);
	apply_expands(t);
}
