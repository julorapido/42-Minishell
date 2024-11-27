/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:05:20 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/27 18:22:41 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_outfile
{
    char    *f_name;
    bool    is_append;
}           t_outfile;

typedef struct s_command
{
    char        *v;
    char        *infile;
    t_outfile   outfiles[300];
    int         outfile_ix;
    bool        piped_in;
    bool        heredoc;
}       t_command;

static void p_commands(t_command *cmds, int l)
{
    for(int i = 0; i < l; i++)
    {
        printf("-> %s [%s: %s] [out: ", 
            cmds[i].v, 
            cmds[i].heredoc ? "heredoc" : "in",
            (!(cmds[i].infile)) ? (i > 0 ? "|" : "STD_IN") : (cmds[i].infile)
        );
        for(int j = 0; j < cmds[i].outfile_ix; j++)
            printf("%s%s", 
                cmds[i].outfiles[j].is_append ? ">>" : ">", 
                cmds[i].outfiles[j].f_name
            );
        if(!cmds[i].outfile_ix)
            printf("|");
        printf("]\n");
    }
}

void	fdp_parsing(char *cmd)
{
    int i;
    int l;

    char **s = ft_split_quotes(cmd, '|', 0);
    if (quote_errors(cmd) != -1 || parse_errors(ft_split_quotes(cmd, ' ', 0)) != '\0')
    {
        printf("zsh: parse error near '%c' \n", parse_errors(s));
        return;
    }
    i = 0;
    l = 0;
    while(s[l])
        l++;
    t_command   *cmds;
    cmds = (t_command*) malloc(l * sizeof(t_command));
    i = 0;
    while(s[i])
    {
        if(ft_m_strchr_i(s[i], '>', '<') != -1)
        {
            cmds[i].outfile_ix = 0;
            int a = 0;
            char *set = ft_strdup("><");
            t_mltsplit *sq = ft_multisplit(s[i], set);
            while((sq[a]).s)
            {
                /*if(a != 0)
                    printf("{%s: %c} ", sq[a].s, set[(sq[a]).ix]);
                else
                    printf("{%s} ", sq[a].s);*/
                if(a == 0)
                    cmds[i].v = (sq[a]).s;   
                else
                {
                    if(set[(sq[a]).ix] == '<')
                    {
                        if(a - 1 > 0)
                            if(set[(sq[a - 1]).ix] == '<')
                                cmds[i].heredoc = true; 
                        cmds[i].infile = (sq[a]).s;
                    }
                    if(set[(sq[a]).ix] == '>' && ft_strlen(sq[a].s))
                    {
                        if(a - 1 > 0)
                            if((!ft_strlen(sq[a - 1].s)) && (set[(sq[a - 1]).ix] == '>'))
                                cmds[i].outfiles[cmds[i].outfile_ix].is_append = true;
                        cmds[i].outfiles[cmds[i].outfile_ix].f_name = (sq[a]).s; 
                        cmds[i].outfile_ix++;    
                    }
                }
                a++;
            }
            printf("\n");
        }else
            cmds[i].v = s[i];
        i++;
    }
    p_commands(cmds, l);
}
