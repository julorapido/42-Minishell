/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:46:03 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/02 17:01:19 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static enum TOKEN_TYPE	switcher(char *token)
{
	if (strlen(token) == 1 && *token == '<')
		return (LESS);
	if (strlen(token) == 1 && *token == '>')
		return (GREAT);	
	if (strlen(token) == 1 && *token == ';')
		return (SEPARATOR);	
	if (*token == '|')
		return (PIPE);
	if (strlen(token) == 2 && ft_strcmp(token, "<<") == 2)
		return (LESS_LESS);
	if (strlen(token) == 2 && ft_strcmp(token, ">>") == 2)
		return (GREAT_GREAT);
	if (strlen(token) > 1 && *token == '$')
		return (ARGUMENT);
	if (strlen(token) > 1 && *token == '-')
		return (COMMAND_FLAG);
	if (strlen(token) > 1 && !strchr(token, '\"'))
		return (COMMAND);
	return (COMMAND);
}

static char	*fn_realloc_strcat(char *filled_str, char *cncat_str)
{
	int		i;
	int		j;
	char	*temp;

	temp = filled_str;;
	filled_str = (char *) malloc((ft_strlen(filled_str) + ft_strlen(cncat_str) + 2));
	i = 0;
	while(temp[i])
	{
		filled_str[i] = temp[i];
		i++;
	}
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

int	parse_command(char *cmd, token **cmd_tokens)
{	
	char			**s_cmds;
	char			*s_quote;
	token			*t;
	enum TOKEN_TYPE	big_t;
	int				i;

	s_cmds = ft_split(cmd, ' ');
	i = 0;
	while (s_cmds[i])
	{
		if(*s_cmds[i] == '\"')
		{	
			s_quote = malloc(ft_strlen(s_cmds[i]) + 1);	
			int o = i;
			ft_strlcpy(s_quote, s_cmds[i], ft_strlen(s_cmds[i]) + 1);
			if(s_cmds[i + 1])
			{
				i++;
				while(ft_strchr(s_cmds[i], '\"') == NULL)
				{
					s_quote = fn_realloc_strcat(s_quote, s_cmds[i]);
					i++;
					if(!s_cmds[i])
			cmd = commands[i];
						break;
				}
				if(i > o && s_cmds[i])
					s_quote = fn_realloc_strcat(s_quote, s_cmds[i]);
				int p = (!s_cmds[i]) ? (i - 1) : (i);
				while (p > o)
				{
					free(s_cmds[p]);
					p--;
				}
			}else
				free(s_cmds[i]);
			t = token_new(s_quote, QUOTE);
			token_push(cmd_tokens, t);
			i++;
		}
		else
		{
			big_t = switcher(s_cmds[i]);
			t = token_new(s_cmds[i], big_t);
			token_push(cmd_tokens, t);
			i++;
		}
	}
	return(0);
}


int parse_to_executor(token **cmd_tokens)
{
	token	*t;
	t_cmd	*commands;
	t_cmd	cmd;
	int		i;

	t = token_last(*cmd_tokens);
	commands = (t_cmd *) malloc(sizeof(t_cmd) * 10);
	printf("---------- EXECUTOR-COMMANDS ----------\n");	
	printf("LAST TOKEN : [%s] \n", token_type_to_str(t->t));
	printf("TOKEN VALUE : %s \n", t->cmd);
	printf("\n");
	while(t)
	{
		cmd = commands[i];
		enum TOKEN_TYPE cmd_spliter[4] = {PIPE, LESS, LESS_LESS, GREAT_GREAT, GREAT};
		if(t->t == PIPE || t->t == LESS || t->t == LESS_LESS || t->t == GREAT_GREAT || t->t == GREAT)
		{
			if(t->t == GREAT_GREAT || t->t == GREAT)
			{
				cmd
			}
			i++;
		}
		printf("[%s] -> ", token_type_to_str(t->t));
		t = t->prev;
	}
	return (0);
}
