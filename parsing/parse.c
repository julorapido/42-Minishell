/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:05:20 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/29 11:54:48 by jsaintho         ###   ########.fr       */
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

    i = 0;
    while(s[i].s)
        i++;
    if(i == 0)
        return (str_token);
    i = 1;
    if(!cmd_to_modify->command)
    {
        cmd_to_modify->command = ft_strdup(s[1].s);
        i++;
    }
    while(s[i].s)
    {
        cmd_to_modify->command = ft_strjoin_free(
            cmd_to_modify->command,  ft_strjoin_free(ft_strdup(" "), s[i].s)
        );
        i++;
    }
    return (s[0].s);
}

static void apply_gauthier(t_minishell *t)
{
    
}


void	fdp_parsing(char *cmd, t_minishell *t)
{
    int         i;
    int         a;

    t_mltsplit *s = ft_multisplit(cmd, "|");
    t->cmds = (t_cmd*) malloc((*s).mltsplit_l * sizeof(t_cmd));
    i = 0;
    while(s[i].s)
    {
        if(ft_m_strchr_i(s[i].s, '>', '<') != -1)
        {
            a = 0;
            char *set = ft_strdup("><");
            t_mltsplit *sq = ft_multisplit(s[i].s, set);
            while((sq[a]).s)
            {
                if(a == 0)
                    t->cmds[i].command = ft_strdup((sq[a]).s);   
                else
                {
                    if(set[(sq[a]).ix] == '<')
                        if(a - 1 > 0)
                            if(set[(sq[a - 1]).ix] == '<')
                                t->cmds[i].files[t->cmds[i].f_i].heredoc = true; 
                    if(set[(sq[a]).ix] == '>' && ft_strlen(sq[a].s))
                        if(a - 1 > 0)
                            if((!ft_strlen(sq[a - 1].s)) && (set[(sq[a - 1]).ix] == '>'))
                                t->cmds[i].files[t->cmds[i].f_i].append = true;
                    if(ft_strlen(sq[a].s))
                    {
                        t->cmds[i].files[t->cmds[i].f_i].f_name = handle_spaces((sq[a]).s, &(t->cmds[i]));
                        t->cmds[i].files[t->cmds[i].f_i]._out = (set[(sq[a]).ix] == '<') ? false : true;
                        t->cmds[i].f_i++;
                    }
                }
                a++;
            }
        }else
            t->cmds[i].command = s[i].s;
        i++;
    }
    t->cmd_count = (*s).mltsplit_l;
    p_commands(t->cmds, (*s).mltsplit_l);
}
