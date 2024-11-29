/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:05:20 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/29 15:29:00 by jsaintho         ###   ########.fr       */
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
    t_mltsplit *s = ft_multisplit(str_token, " ");
    int     i;
    int     c;

    c = 0;
    i = 0;
    while(s[c].s && ft_strlen(s[c].s) == 0)
        c++;
    while(s[i].s)
        i++;
    if(i == 0)
        return (str_token);
    i = c + 1;
    if(!cmd_to_modify->command)
    {
        if(ft_strlen(s[i].s) == 0)
            s[i].s = ft_strdup(" ");
        cmd_to_modify->command = ft_strdup(s[i].s);
        i++;
    }
    while(s[i].s)
    {
        if(ft_strlen(s[i].s) == 0)
            s[i].s = ft_strdup(" ");
        cmd_to_modify->command = ft_strjoin_free(
            cmd_to_modify->command,  ft_strjoin_free(ft_strdup(" "), s[i].s)
        );
        i++;
    }
    return (s[c].s);
}

static void apply_gauthier(t_minishell *t)
{
    int     i;
    int     j;
    t_cmd   *c;

    i = 0;
    while ((size_t)i < t->cmd_count)
    {
        c = &(t->cmds[i]);
        j = 0;
        while (j < (c)->f_i)
        {
            if (c->files[j]._out)
                c->output = c->files[j].f_name;
            else
                c->input = c->files[j].f_name;
            j++;
        }
        c->is_piped_in = MACRO_ZERO(c->n_in > 0);
        c->is_piped_out = MACRO_ZERO(c->n_out > 0);
        i++;
    }
}


void	fdp_parsing(char *cmd, t_minishell *t)
{
    int         i;
    int         a;

    t_mltsplit *s = ft_multisplit(cmd, "|");
    t->cmds = (t_cmd*) malloc((*s).mltsplit_l * sizeof(t_cmd));
    t->cmd_count = (*s).mltsplit_l;
    i = 0;
    while(s[i].s)
    {
        printf("S[%d].s = %s \n", i, s[i].s);
        if(ft_m_strchr_i(s[i].s, '>', '<') != -1)
        {
            a = 0;
            t_mltsplit *sq = ft_multisplit(s[i].s, t->set);
            while((sq[a]).s)
            {
                printf("-s[%d].s = {%s}{%c} \n",i, sq[a].s, t->set[(sq[a]).ix]);
                if(a == 0)
                    t->cmds[i].command = ft_strdup((sq[a]).s);   
                else
                {
                    if(t->set[(sq[a]).ix] == '<' && (a - 1 > 0))
                        if(t->set[(sq[a - 1]).ix] == '<')
                            t->cmds[i].files[t->cmds[i].f_i].heredoc = true; 
                    if(t->set[(sq[a]).ix] == '>' && ft_strlen(sq[a].s) && (a - 1 > 0))
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
                }
                a++;
            }
        }else
            t->cmds[i].command = s[i].s;
        i++;
    }
    apply_gauthier(t);
    p_commands(t->cmds, (*s).mltsplit_l);
}
