/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:55:36 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/24 17:31:01 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	fn_revstr(char *up_s)
{
	char	**s_p;
	char	*s;
	int 	i;
	int		j;
 	int		k;

	k = 0;
	i = 0;
	if (!up_s || up_s == NULL || ft_strlen(up_s) <= 1)
		return ;
	s_p = ft_split(up_s, ' ');
	while (s_p[i])
		i++;
	i--;
	while (i >= 0)
	{
		s = s_p[i];
		j = 0;
		while (s[j] != '\0')
		{
			up_s[k] = s[j];
			j++;
			k++;
		}
		up_s[k] = ' ';
		k++;
		i--;
	}
	up_s[k - 1] = '\0';
	i = 0;
	while (s_p[i])
	{
		free(s_p[i]);
		i++;
	}
	free(s_p);
}



enum TOKEN_TYPE	switcher(char *tken, token **t_l)
{	
	int		a;
	char	*s;
	if (strlen(tken) >= 2 && (ft_strchr(tken, '>') || ft_strchr(tken, '<') || ft_strchr(tken, '|')))
	{
		a = ft_m_strchr_i(tken, '>', '<');
		while (a != -1)
		{
			s = ft_substr(tken, 0, a);
			if (ft_strlen(s))	
				token_push(t_l, token_new(s, COMMAND));
			else
				free(s);	
			token_push(t_l, token_new("", char_to_token(tken[a])));	
			tken = ft_substr(tken, a + 1, ft_strlen(tken));
			a = ft_m_strchr_i(tken, '>', '<');
		}	
		if (ft_strlen(tken) > 0)
			token_push(t_l, token_new(tken, COMMAND));
		else
			free(tken);
		return (-1);
	}
	if (strlen(tken) == 1 || *tken == '|')
		return (char_to_token(*tken));
	if (strlen(tken) == 2 && (*tken == '<' && tken[1] == '<'))
		return (LESS_LESS);
	if (strlen(tken) == 2 && (*tken == '>' && tken[1] == '>'))
		return (GREAT_GREAT);
	return (COMMAND);
}

char	*fn_realloc_strcat(char *filled_str, char *cncat_str, int space_it)
{
	int		i;
	int		j;
	char	*temp;

	temp = filled_str;	
	filled_str = (char *) ft_calloc((ft_strlen(temp) + ft_strlen(cncat_str) +  ((space_it) ? (2) : (1))), sizeof(char));
	i = 0;
	while(temp[i])
	{
		filled_str[i] = temp[i];
		i++;
	}
	free(temp);
	if(space_it)
		filled_str[i++] = ' ';
	j = 0;
	while (cncat_str[j])
	{
		filled_str[i + j] = cncat_str[j];
		j++;
	}	
	filled_str[i + j] = '\0';	
	return (filled_str);
}


char	*cmd_remove_lstspace(char *s)
{
	int		i;
	int		j;
	int		l;
	char	*new_s;

	if (!s || s[0] == '\0' || !(*s))
		return (NULL);
	i = 0;
	l = ft_strlen(s);
	if (s[l - 1] == ' ')
		i = 1;
	new_s = (char *) malloc(((l - i) + 1) * sizeof(char));
	if (!new_s)
		return (NULL);
	j = 0;
	while (j < l - i)
	{
		new_s[j] = s[j];
		j++;
	}
	new_s[j] = '\0';
	return (new_s);
}

void	print_commands(t_minishell *t_m)
{
	int		i;
	t_cmd	*cmd__;

	i = 0;
	printf("---------- EXECUTOR-COMMANDS ----------\n");
	while((size_t)(i) < t_m->cmd_count)
	{
		if(!(&(t_m->commands[i])))
			break ;
		cmd__ = &(t_m->commands[i]);
		printf("-Command %d [cmd: %s| in: %s%s| out: %s]", i,
				cmd__->command,
				cmd__->is_heredoc ? "(heredoc <<) ": "",
				cmd__->input, cmd__->output
		);
		if(&(t_m->commands[i + 1]) && ((&(t_m->commands[i + 1]))->input))
		{
			if((cmd__->n_redirections > 0) && (ft_strcmp((&(t_m->commands[i + 1]))->input , "pipe") == 0) )
				printf(" [piped out |] ");
		}
		if (cmd__->n_redirections > 1)
		{
			printf(" [%d out_files] ", cmd__->n_redirections);
			//for(int i = 0; i < cmd__->n_redirections; i++)
			//	printf("%s%s", cmd__->appends[i] == 1 ? ">>": ">", i < cmd__->n_redirections - 1 ? ", "  : "");
			//printf("} ]");
			if(cmd__->is_append)
				printf("[>> append]");
		}
		if(cmd__->is_append)
			printf(" [>> append] ");
		printf("\n");
		i++;
	}
}

void	apply_commands_reverse(t_minishell	*t_m)
{
	int		i;
	t_cmd	temp;

	i = 0;
	while ((size_t)(i) < t_m->cmd_count / 2)
	{
		temp = t_m->commands[i];

		t_m->commands[i] = t_m->commands[(t_m->cmd_count - 1) - i];
		t_m->commands[(t_m->cmd_count - 1) - i] = temp;
		i++;
	}
}

/*
void	apply_space_removal(t_minishell *t_m)
{
	int		i;
	t_cmd	*cmd__;
	char	*temp_cmd;

	i = 0;
	while ((size_t)(i) < t_m->cmd_count)
	{
		cmd__ = &(t_m->commands[i]);
		if (!(cmd__->command))
			break;
		temp_cmd = cmd_remove_lstspace(cmd__->command);
		cmd__->command = temp_cmd;
		i++;
	}
}

void	apply_is_piped_out(t_minishell *t_m)
{
	int		i;
	t_cmd	*cmd__;

	i = 0;
	while ((size_t)(i) < t_m->cmd_count)
	{
		if(!(&(t_m->commands[i])))
			break ;
		cmd__ = &(t_m->commands[i]);
		if (&(t_m->commands[i + 1]) && ((&(t_m->commands[i + 1]))->input))
		{
			//if ((cmd__->n_redirections > 0) && (ft_strcmp((&(t_m->commands[i + 1]))->input , "pipe") == 0) )
				// cmd__->is_piped_out = true;
		}
		i++;
	}
}
*/
