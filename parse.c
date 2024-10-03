/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:46:03 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/03 17:41:07 by jsaintho         ###   ########.fr       */
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
	if (strlen(token) == 2 && (*token == '<' && token[1] == '<'))
		return (LESS_LESS);
	if (strlen(token) == 2 && (*token == '>' && token[1] == '>'))
		return (GREAT_GREAT);
	if (strlen(token) > 1 && *token == '$')
		return (ARGUMENT);
	if (strlen(token) > 1 && *token == '-')
		return (COMMAND_FLAG);
	if (strlen(token) > 1 && !strchr(token, '\"'))
		return (COMMAND);
	return (COMMAND);
}

static char	*fn_realloc_strcat(char *filled_str, char *cncat_str, int space_it)
{
	int		i;
	int		j;
	char	*temp;

	temp = filled_str;
	filled_str = (char *) malloc((ft_strlen(filled_str) + ft_strlen(cncat_str) +  space_it ? 2 : 1));
	i = 0;
	while(temp[i])
	{
		filled_str[i] = temp[i];
		i++;
	}
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


static void	fn_revstr(char *up_s)
{
	char	**s_p;
	char	*s;
	int 	i;
	int		j;
 	int		k;

	k = 0;
	i = 0;
	s_p = ft_split(up_s, ' ');
	while (s_p[i])
		i++;
	i--;
	while (i >= 0)
	{
		s = s_p[i];
		j = 0;
		while(s[j])
		{	
			up_s[k] = s[j];
			j++;
			k++;
		}
		up_s[k] = ' ';
		k++;
		i--;	
	}
	up_s[k] = '\0';
	i = -1;
	while (s_p[i++])
		free(s_p[i]);
	
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
					s_quote = fn_realloc_strcat(s_quote, s_cmds[i], 1);
					i++;
					if(!s_cmds[i])
						break;
				}
				if(i > o && s_cmds[i])
					s_quote = fn_realloc_strcat(s_quote, s_cmds[i], 1);
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

static bool is_operator(enum TOKEN_TYPE *t, bool only_redirections)
{
	const enum TOKEN_TYPE cmd_spliter[6] = {LESS, LESS_LESS, GREAT_GREAT, GREAT, PIPE, SEPARATOR};
	int	i;

	i = 0;
	while (i < ((only_redirections) ? (4) : (6)))
	{
		if(*t == cmd_spliter[i])
			return (true);
		i++;
	}
	return (false);
}

int parse_to_executor(token **cmd_tokens)
{
	token	*t;
	t_cmd	*commands;
	t_cmd	*cmd__;
	int		i, c;
	int 	in = 0, ou = 0;
	bool 	sp = false, lst_was_pipe = false; 

	i = 0;
	t = token_last(*cmd_tokens);
	commands = (t_cmd *) malloc(sizeof(struct s_cmd) * 10);	
	printf("---------- EXECUTOR-COMMANDS ----------\n");	
	// printf("LAST-TOKEN(start) [VALUE : %s] [TYPE : %s] \n", t->cmd, token_type_to_str(t->t));
	// printf("\n");
	while(t)
	{
		cmd__ = &commands[i];
		if((t->t == COMMAND || t->t == COMMAND_FLAG))
		{
			if(t->prev)
				if((t->prev)->t == GREAT || (t->prev)->t == GREAT_GREAT || (t->prev)->t == LESS || (t->prev)->t == LESS_LESS)
					sp = true;
			// concat current cmd.command with (cmd or cmd_flags)
			if(!sp)
			{
				if(!(cmd__->command))
				{
					cmd__->command = (char *) malloc((ft_strlen(t->cmd) + 1) * sizeof(char));	
					ft_strlcpy(cmd__->command, t->cmd, ft_strlen(t->cmd) + 1);	
				}else	
					cmd__->command = fn_realloc_strcat(cmd__->command, t->cmd, 1);
			}
		}	
		// redirections [<] [<<] [>] [>>]
		if (t->t == GREAT_GREAT || t->t == GREAT)
		{
			cmd__->output = (t->next)->cmd;
			ou++;
		}
		if (t->t == LESS_LESS || t->t == LESS)
		{
			cmd__->input = (t->next)->cmd;
			in++;
		}
		// pipe [|] and separator [;]
		if (t->t == SEPARATOR || t->t == PIPE || t->prev == NULL)
		{
			if(!in)
				cmd__->input = (t->t == SEPARATOR || t->prev == NULL) ? "STD_IN" : "pipe";
			if(!ou)
			{
				if(lst_was_pipe)
				{
					cmd__->output = "pipe";	
					lst_was_pipe = false;
				}
				else
					cmd__->output = "STD_OUT";	
			}
			if (t->t == PIPE || t->t == SEPARATOR)
			{	
				lst_was_pipe = (t->t == PIPE) ? (true) : (false);
				i++;
				ou = in = 0;
			}
			fn_revstr(cmd__->command);
		}
		sp = false;
		t = t->prev;
	}
	c = 0;
	while(i >= 0)
	{
		cmd__ = &commands[i];
		printf("-COMMAND %d [cmd: %s| in: %s| out: %s] \n", c, cmd__->command, cmd__->input, cmd__->output);
		i--;
		c++;
	}
	return (0);
}
