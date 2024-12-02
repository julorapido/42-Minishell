/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:05:20 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/02 17:29:01 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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


static char *handle_spaces(char *str_token, t_cmd *cmd_to_modify)
{
    t_mltsplit  *s;
    int         i;
    int         c;
    char        *a;

    c = 0;
    i = 0;
    s = ft_multisplit(str_token, " ");
    while(s[c].s && ft_strlen(s[c].s) == 0)
        c++;
    while(s[i].s)
        i++;
    if(i == 0)
        return (ft_strdup(str_token));
    i = c + 1;
    while(s[i].s)
    {
        if(ft_strlen(s[i].s) == 0)
            s[i].s = ft_strdup(" ");
        cmd_to_modify->command = ft_strjoin_free(
            cmd_to_modify->command,  ft_strjoin_free(ft_strdup(" "), ft_strdup(s[i].s))
        );
        i++;
    }
    a = ft_strdup(s[c].s);
    return (free_multisplit(s), a);
}



void	fdp_parsing(char *cmd, t_minishell *t)
{
    int         i;
    int         a;
    bool        T;

    t_mltsplit *s = ft_multisplit(cmd, "|");
    t->cmds = (t_cmd*) malloc((*s).mltsplit_l * sizeof(t_cmd));
    t->cmd_count = (*s).mltsplit_l;
    i = 0;
    while(s[i].s)
    {
        if(ft_m_strchr_i(s[i].s, '>', '<') != -1)
        {
            t_mltsplit *sq = ft_multisplit(s[i].s, t->set);
            T = (*s[i].s) == '>' || (*s[i].s == '<');
            a = T ? 0 : 1;
            t->cmds[i].command = T ? ft_strdup(" ") : ft_strdup((sq[0]).s);
            while((sq[a]).s)
            {
                if(t->set[(sq[a]).ix] == '<' && (a - 1 >= 0))
                    if(t->set[(sq[a - 1]).ix] == '<')
                        t->cmds[i].files[t->cmds[i].f_i].heredoc = true; 
                if(t->set[(sq[a]).ix] == '>' && ft_strlen(sq[a].s) && (a - 1 >= 0))
                    if((!ft_strlen(sq[a - 1].s)) && (t->set[(sq[a - 1]).ix] == '>'))
                        t->cmds[i].files[t->cmds[i].f_i].append = true;
                if(ft_strlen(sq[a].s))
                {
                    t->cmds[i].files[t->cmds[i].f_i].f_name = handle_spaces((sq[a]).s, &(t->cmds[i]));
                    t->cmds[i].files[t->cmds[i].f_i]._out = (t->set[(sq[a]).ix] == '<') ? false : true;
                    t->cmds[i].n_out +=  (t->set[(sq[a]).ix] == '>') ? 1 : 0;
                    t->cmds[i].n_in +=  (t->set[(sq[a]).ix] == '<') ? 1 : 0;
                    t->cmds[i].f_i++;
                }
                a++;
            }
            free_multisplit(sq);
        }else
            t->cmds[i].command = ft_strdup(s[i].s);
        i++;
    }
    free_multisplit(s);
    p_commands(t->cmds, t->cmd_count);
}