/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:46:03 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/10 17:23:10 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static enum TOKEN_TYPE	switcher(char *tken, token **t_l)
{	
	int	a;
	if (strlen(tken) >= 2 && (ft_strchr(tken, '>') || ft_strchr(tken, '<') || ft_strchr(tken, '|')))
	{
		a = ft_m_strchr_i(tken, '>', '<');
		while (a != -1 && !is_parse_error(tken))
		{		
			token_push(t_l, token_new(ft_substr(tken, 0, a) , COMMAND));
			token_push(t_l, token_new("", char_to_token(tken[a])));
			/*printf("Ecumed: [%s], Used: [%s], Token: [%s] \n",
					ft_substr(tken, a + 1, ft_strlen(tken)),
					ft_substr(tken, 0, a),
					token_type_to_str( char_to_token(tken[a]))
				);*/
			tken = ft_substr(tken, a + 1, ft_strlen(tken));	
			a = ft_m_strchr_i(tken, '>', '<');
		}
		if(ft_strlen(tken) > 0 && !is_parse_error(tken))
			token_push(t_l, token_new(tken, COMMAND));		
		return (-1);
	}
	else
	{
		if (strlen(tken) == 1 || *tken == '|')
			return (char_to_token(*tken));
		if (strlen(tken) == 2 && (*tken == '<' && tken[1] == '<'))
			return (LESS_LESS);
		if (strlen(tken) == 2 && (*tken == '>' && tken[1] == '>'))
			return (GREAT_GREAT);
		if (strlen(tken) > 1 && *tken == '$')
			return (ARGUMENT);
		if (strlen(tken) > 1 && *tken == '-')
			return (COMMAND_FLAG);
		if (strlen(tken) > 1 && !strchr(tken, '\"'))
			return (COMMAND);
	}
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



// PARSE [COMMAND] TO [TOKENS]
int	parse_tokens(char *cmd, token **cmd_tokens, t_minishell *t_m)
{	
	char			**s_cmds;
	char			*s_quote;
	token			*t;
	enum TOKEN_TYPE	big_t;
	// un-spaced special	
	enum TOKEN_TYPE	saved_t;
	char			*saved_s = NULL;
	int				i;
	int				s;

	s_cmds = ft_split(cmd, ' ');
	s = 0;
	while(s_cmds[s])
		s++;
	i = 0;
	while (s_cmds[i] != NULL && i < s)
	{		
		// QUOTE ""
		if(*s_cmds[i] == '\"')
		{	
			s_quote = malloc(ft_strlen(s_cmds[i]) + 1);	
			int o = i;
			ft_strlcpy(s_quote, s_cmds[i], ft_strlen(s_cmds[i]) + 1);
			if(s_cmds[i + 1] 
				&& (s_cmds[i][ft_strlen(s_cmds[i]) - 1] != '\"')
				&& 	ft_strchr(ft_substr(s_cmds[i], 1, ft_strlen(s_cmds[i])), '\"')
				&& 	ft_strchr(ft_substr(s_cmds[i], 1, ft_strlen(s_cmds[i])), ' ')
			){	
				i++;
				// CONCATENATE UNTIL NEW "
				while(ft_strchr(s_cmds[i], '\"') == NULL)
				{
					s_quote = fn_realloc_strcat(s_quote, s_cmds[i], 1);
					i++;
					if(!s_cmds[i])
						break;
				}

				if(i != o + 1 && s_cmds[i] != NULL)
				{	
					s_quote = fn_realloc_strcat(s_quote, s_cmds[i], 1);
					i++;
				}
				int p = (!s_cmds[i]) ? (i - 1) : (i);
				if(i != o + 1)
				{
					while (p > o)
					{
						free(s_cmds[p]);
						p--;
					}
				}
			}else if ( ft_strchr(ft_substr(s_cmds[i], 1, ft_strlen(s_cmds[i])), '\"')
					&& (s_cmds[i][ft_strlen(s_cmds[i]) - 1] != '\"')
			){	
				int quote_ix = ft_m_strchr_i(ft_substr(s_cmds[i], 1, ft_strlen(s_cmds[i]) - 1), '\"', '\"') + 2;
				s_quote = ft_substr(s_cmds[i], 0, quote_ix);	
				token_push(cmd_tokens, token_new(s_quote, QUOTE));
				s_cmds[i] = ft_substr(s_cmds[i], quote_ix, ft_strlen(s_cmds[i]));
				continue;
			}else
				free(s_cmds[i]);	
			t = token_new(s_quote, QUOTE);
			token_push(cmd_tokens, t);
			i++;
		}
		// NO QUOTE _
		else
		{
			// CMD HAS NO [; |] or is strict equal to [>, <, |, ;]
			if( ft_strlen(s_cmds[i]) == 1 || (!ft_strchr(s_cmds[i], ';') && !ft_strchr(s_cmds[i], '|')))
			{	
				big_t = switcher(s_cmds[i], cmd_tokens);	
				if(big_t != -1)
				{
					t = token_new(s_cmds[i], big_t);
					token_push(cmd_tokens, t);
				}
				i++;
			}
			// CMD HAS SEPARATOR [; |]
			else
			{			
				if(is_parse_error(s_cmds[i]))
				{
					t_m->parse_error_value = s_cmds[i];
					t_m->parse_error = true;
					return(0);
				}else
				{
					int ix = ft_m_strchr_i(s_cmds[i], ';', '|');
					saved_s = ft_substr(s_cmds[i], 0, ix);
					saved_t = s_cmds[i][ix] == ';' ? SEPARATOR : PIPE;
					if ((size_t)(ix + 1) == ft_strlen(s_cmds[i]))
						i++;
					else
						s_cmds[i] = ft_substr(s_cmds[i], ix + 1, ft_strlen(s_cmds[i]));	
					if (ft_strlen(saved_s) > 0)
						token_push(cmd_tokens, token_new(saved_s, switcher(saved_s, cmd_tokens)));
					token_push(cmd_tokens, token_new("", saved_t));
				}
			}
		}
	}		
	return(0);
}




int parse_commands(t_minishell *t_m, token **cmd_tokens)
{
	token	*t;
	t_cmd	*commands;
	t_cmd	*cmd__;
	int		i;
	int 	in = 0, ou = 0;
	bool 	sp = false, lst_was_pipe = false;

	i = 0;
	t = token_last(*cmd_tokens);
	commands = (t_cmd *) malloc(sizeof(struct s_cmd) * MAX_CMDS);
	while(t)
	{
		cmd__ = &commands[i];	
		// commands, command flags and quotes [ls -l "bonjour"]
		if((t->t == COMMAND || t->t == COMMAND_FLAG) || t->t == QUOTE)
		{
			if(t->prev)
				if((t->prev)->t == GREAT || (t->prev)->t == GREAT_GREAT || (t->prev)->t == LESS || (t->prev)->t == LESS_LESS)
					sp = true;
			// concat current cmd.command with (cmd or cmd_flags or quote)
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
			if((t->next) && (t->next)->cmd)
			{
				if(ou > 0)
					cmd__->output = fn_realloc_strcat(cmd__->output, (t->next)->cmd, 1);
				else
					cmd__->output = (t->next)->cmd;
				ou++;
			}	
		}
		if ((t->t == LESS_LESS || t->t == LESS) && (in < 1))
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
				if(cmd__->command)
				{
					cmd__->n_redirections = ou;
					i++;
				}
				ou = in = 0;
			}
			fn_revstr(cmd__->command);
		}
		sp = false;
		t = t->prev;
	}	
	if(i == 0)
		(&commands[0])->n_redirections = ou;
	t_m->commands = commands;
	t_m->cmd_count = i + 1;
	rev_tm_commands(t_m);
	appyl_space_removal(t_m);
	print_commands(t_m);
	return (0);
}
