/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:55:36 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/25 17:51:47 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *fn_revaround_quote(char *str)
{
	int		i;
	int		j;
	char	left[ft_strlen(str) / 2];
	char	right[ft_strlen(str) / 2];
	char	*new_s;

	new_s = (char *) malloc(ft_strlen(str));
	j = 0;
	i = 0;	
	while (str[i] != '\0' && str[i] != '\"')
	{
		left[j] = str[i];
		i++;
		j++;
	}
	i++;
	left[j] = '\0';
	while(str[i] != '\0' && i < ft_last_strchr_i(str, '\"', '\"'))
		i++;
	i++;
	j = 0;
	while (str[i])
	{
		right[j] = str[i];
		j++;
		i++;
	}
	right[j] = '\0';
	return (ft_strjoin(ft_strlen(right) ? ft_strdup(right) : "", 
		ft_strjoin(
			ft_substr(str, 
				ft_strlen(left),
				((ft_strlen(str) - ft_strlen(right)) - ft_strlen(left)) 
			),		
			ft_strdup(left)
		)
	));
}

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
	s_p = ft_split_quotes(up_s, ' ', 0);
	while (s_p[i])
	{
		if(ft_m_strchr_i(s_p[i], '\"', '\"') != -1)
			if(*s_p[i] != '\"' || s_p[ft_strlen(s_p[i]) - 1] != '\"')
				s_p[i] = fn_revaround_quote(s_p[i]);
		i++;
	}
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



enum TOKEN_TYPE	switcher(t_minishell *t_m, char *tken, token **t_l)
{	
	int		a;
	char	*s;
	char	*tt;
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
			tt = tken;
			tken = ft_substr(tken, a + 1, ft_strlen(tken));
			free(tt);
			a = ft_m_strchr_i(tken, '>', '<');
		}	
		if (ft_strlen(tken) > 0)
			token_push(t_l, token_new(tken, COMMAND));
		else
			free(tken);
		if(t_m->stocked_token)
			token_push(t_l, t_m->stocked_token);
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
	filled_str = (char *) ft_calloc((ft_strlen(temp) + ft_strlen(cncat_str) +  2), sizeof(char));
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
	new_s = ft_calloc(((l - i) + 1), sizeof(char));
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
		if ((&(t_m->commands[i]))->continue_)
		{
			i++; continue ;
		}
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
		if (cmd__->n_redirections >= 1)
		{
			printf(" [%d out_files] ", cmd__->n_redirections);
			//for(int i = 0; i < cmd__->n_redirections; i++)
			//	printf("%s%s", cmd__->appends[i] == 1 ? ">>": ">", i < cmd__->n_redirections - 1 ? ", "  : "");
			if(cmd__->is_append)
				printf("[>> append]");
		}
		printf("\n");
		i++;
	}
}

void	apply_commands_reverse(t_minishell	*t_m)
{
	int		i;
	t_cmd	temp;

	i = 0;
	if (t_m->cmd_count <= 1)
		return ;
	while ((size_t)(i) < t_m->cmd_count / 2)
	{
		temp = t_m->commands[i];

		t_m->commands[i] = t_m->commands[(t_m->cmd_count - 1) - i];
		t_m->commands[(t_m->cmd_count - 1) - i] = temp;
		i++;
	}
}

void	apply_is_stds(t_minishell *t_m)
{
	int		i;

	i = 0;
	while ((size_t)(i) < t_m->cmd_count)
	{
		if (!ft_strcmp((&t_m->commands[i])->input, "STD_IN"))
			(&(t_m->commands[i]))->is_stdin = true;
		if (!ft_strcmp((&t_m->commands[i])->output, "STD_OUT"))
			(&(t_m->commands[i]))->is_stdout = true;
		if (!ft_strcmp((&t_m->commands[i])->output, "pipe"))
			(&(t_m->commands[i]))->is_piped_out = true;
		if (!ft_strcmp((&t_m->commands[i])->input, "pipe"))
		(&(t_m->commands[i]))->is_piped_in = true;
		
		i++;
	}
}

void parse_free(t_minishell *t_m)
{
	// size_t	i;

	// i = 0;
	if(t_m->splt_cat)
		free(t_m->splt_cat);
	for (int i = 0; i < MAX_CMDS; i++)
	{
		if(t_m->commands[i].command)
			ft_memdel(t_m->commands[i].command);
		if(t_m->commands[i].output)
			ft_memdel(t_m->commands[i].output);
		// if(t_m->commands[i].input)
		// 	free(t_m->commands[i].input);
	}
	//return;
	free(t_m->commands);
	free_tokens(t_m->cmd_tokens);
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
