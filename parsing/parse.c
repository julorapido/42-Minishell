/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:46:03 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/21 18:01:35 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	parse_errors(char *cmd, t_minishell *t_m)
{
	char	*s_cmds;
	int		i;
	int		q;
	int		l;

	t_m->parse_error = false;
	s_cmds = ft_splitcat(ft_split(cmd, ' '));	
	l = ft_strlen(s_cmds);
	if(l == 0)
		return (0);
	i = 0;
	q = 0;
	if ((is_char_operator(s_cmds[0]) && ft_strlen(s_cmds) == 1) || s_cmds[0] == '|')
		return (0, t_m->e_v[0] = '/', t_m->e_v[1] = 'n', t_m->parse_error = true);
	while (i < l)
	{
		if (s_cmds[i] == '\"')
			q++;
		if (i + 1 < l)
		{
			// > ;
			if (s_cmds[i] == '>' && s_cmds[i + 1] == ';')
				 t_m->parse_error = true;
			// triple >>> ;;; <<< /// 
			if (i + 1 + 1 < l)
				if (is_char_operator(s_cmds[i]))
					if (s_cmds[i] == s_cmds[i + 1] && s_cmds[i + 1] == s_cmds[i + 2])
						t_m->parse_error = true;
			// >| <; |>
			if (is_char_operator(s_cmds[i + 1]) && is_char_operator(s_cmds[i]))
				if (s_cmds[i + 1] != s_cmds[i] && !(s_cmds[i + 1] == '|' || s_cmds[i + 1] == ';'))
					t_m->parse_error = true;
			if (t_m->parse_error == true)
				return (0, t_m->e_v[0] = s_cmds[i], t_m->e_v[1] = s_cmds[i + 1], t_m->parse_error = true);
		}
		i++;
	}	
	if (q % 2 != 0)
		return (0, t_m->e_v[0] = '\"', t_m->parse_error = true);
	if (i > 0)
		if (s_cmds[i - 1] == '|' || s_cmds[i - 1] == '>' || s_cmds[i - 1] == '<')
			return (0,t_m->e_v[0] = s_cmds[i - 1], t_m->e_v[1] = NULL, t_m->parse_error = true);
}


static enum TOKEN_TYPE	switcher(char *tken, token **t_l)
{	
	int	a;
	if (strlen(tken) >= 2 && (ft_strchr(tken, '>') || ft_strchr(tken, '<') || ft_strchr(tken, '|')))
	{
		a = ft_m_strchr_i(tken, '>', '<');
		while (a != -1)
		{	
			if (ft_strlen(ft_substr(tken, 0, a)))
				token_push(t_l, token_new(ft_substr(tken, 0, a) , COMMAND));
			token_push(t_l, token_new("", char_to_token(tken[a])));
			tken = ft_substr(tken, a + 1, ft_strlen(tken));	
			a = ft_m_strchr_i(tken, '>', '<');
		}
		if (ft_strlen(tken) > 0)
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
	}
	return (COMMAND);
}

char	*fn_realloc_strcat(char *filled_str, char *cncat_str, int space_it)
{
	int		i;
	int		j;
	char	*temp;

	temp = filled_str;
	filled_str = (char *) malloc((ft_strlen(filled_str) + ft_strlen(cncat_str) +  space_it ? 2 : 1));
	i = 0;
	while (temp[i])
	{
		filled_str[i] = temp[i];
		i++;
	}	
	if (space_it){
		filled_str[i + 1] = ' ';i++;}
	j = 0;
	while (cncat_str[j])
	{
		filled_str[i + j] = cncat_str[j];
		j++;
	}	
	printf("PUT NULL CHARACTER ON [%s] at [%d] \n", filled_str, i + j);
	filled_str[i + j] = '\0';	
	return (filled_str);
}

static void parse_quote(token **cmd_tokens, char **s_cmds, int *i)
{
	char	*s_quote;
	int		o;
	int		p;
	char	*al_s1;
	token	*t;
	int		quote_ix;

	s_quote = malloc(ft_strlen(s_cmds[(*i)]) + 1);	
	o = (*i);
	ft_strlcpy(s_quote, s_cmds[(*i)], ft_strlen(s_cmds[(*i)]) + 1);
	al_s1 = ft_substr(s_cmds[(*i)], 1, ft_strlen(s_cmds[(*i)]));
	if(s_cmds[(*i) + 1] && (s_cmds[(*i)][ft_strlen(s_cmds[(*i)]) - 1] != '\"') && ft_strchr(al_s1, '\"') && ft_strchr(al_s1, ' '))
	{	
		(*i)++;
		// CONCATENATE UNTIL NEW "
		while(ft_strchr(s_cmds[(*i)], '\"') == NULL)
		{
			s_quote = fn_realloc_strcat(s_quote, s_cmds[(*i)], 1);
			(*i)++;
			if(!s_cmds[(*i)])
				break;
		}
		if((*i) != o + 1 && s_cmds[(*i)] != NULL)
		{	
			s_quote = fn_realloc_strcat(s_quote, s_cmds[(*i)], 1);
			(*i)++;
		}
		p = (!s_cmds[(*i)]) ? ((*i) - 1) : (*i);
		if((*i) != o + 1)
		{
			while (p > o)
			{
				free(s_cmds[p]);
				p--;
			}
		}
	}
	else if ( ft_strchr(ft_substr(s_cmds[(*i)], 1, ft_strlen(s_cmds[(*i)])), '\"')
		&& (s_cmds[(*i)][ft_strlen(s_cmds[(*i)]) - 1] != '\"')
	){	
		quote_ix = ft_m_strchr_i(ft_substr(s_cmds[(*i)], 1, ft_strlen(s_cmds[(*i)]) - 1), '\"', '\"') + 2;
		s_quote = ft_substr(s_cmds[(*i)], 0, quote_ix);	
		token_push(cmd_tokens, token_new(s_quote, QUOTE));
		s_cmds[(*i)] = ft_substr(s_cmds[(*i)], quote_ix, ft_strlen(s_cmds[(*i)]));
		return;
	}else
		free(s_cmds[(*i)]);	
	t = token_new(s_quote, QUOTE);
	token_push(cmd_tokens, t);
	(*i)++;
}



// PARSE [TOKENS]
int	parse_tokens(char *cmd, token **cmd_tokens, t_minishell *t_m)
{	 
	char			**s_cmds;	
	char			*saved_s;
	int				i;

	s_cmds = ft_split(cmd, ' ');
	i = 0;
	while (s_cmds[i] != NULL)
	{	
		if(*s_cmds[i] == '\"') // QUOTE ""
			parse_quote(cmd_tokens, s_cmds, &i);
		else // NO QUOTE
		{
			// CMD HAS NO [; |] or is strict equal to [>, <, |, ;]
			if (ft_strlen(s_cmds[i]) == 1 || (!ft_strchr(s_cmds[i], ';') && !ft_strchr(s_cmds[i], '|'))) 
			{
				if (ft_strcmp(s_cmds[i], ">>") == 0)
					token_dbl_push(cmd_tokens, token_new("", GREAT));
				else
					if (switcher(s_cmds[i], cmd_tokens) != -1)
						token_push(cmd_tokens, token_new(s_cmds[i], switcher(s_cmds[i], cmd_tokens)));
				i++;
			}	
			else // CMD HAS SEPARATOR [; |]
			{		
				int ix = ft_m_strchr_i(s_cmds[i], ';', '|');
				saved_s = ft_substr(s_cmds[i], 0, ix);	
				if (ft_strlen(saved_s) > 0)
					token_push(cmd_tokens, token_new(saved_s, switcher(saved_s, cmd_tokens)));
				token_push(cmd_tokens, token_new("", s_cmds[i][ix] == ';' ? SEPARATOR : PIPE));
				if ((size_t)(ix + 1) == ft_strlen(s_cmds[i]))
					i++;
				else
					s_cmds[i] = ft_substr(s_cmds[i], ix + 1, ft_strlen(s_cmds[i]));
			}
		}
	}
	return(0);
}



// PARSE [COMMANDS]
int parse_commands(t_minishell *t_m, token **cmd_tokens)
{
	token	*t;
	t_cmd	*commands;
	t_cmd	*cmd__;
	int		i;
	int 	in = 0, ou = 0, s_ou_count = 0;
	bool 	sp = false, lst_was_pipe = false;

	i = 0;
	t = token_last(*cmd_tokens);
	commands = (t_cmd *) malloc(sizeof(struct s_cmd) * MAX_CMDS);
	for(int i = 0; i< MAX_CMDS ; i ++)
		for(int j = 0; j < MAX_OUTFILES; j++)
			(&commands[i])->appends[j] = 0;
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
		// redirections [>] appends [>>]
		if (t->t == GREAT_GREAT || t->t == GREAT)
		{	
			if(t->prev)
				if((t->prev)->t == GREAT)
					cmd__->appends[s_ou_count] = 1;
			if((t->next) && (t->next)->cmd && (ft_strlen((t->next)->cmd) > 0))
			{	
				if(ou > 0)
				{
					cmd__->output = fn_realloc_strcat(cmd__->output, (t->next)->cmd, 1);
					s_ou_count++;
				}
				else
					cmd__->output = (t->next)->cmd;
				ou++;
			}	
		}
		// input [<] heredoc [<<]
		if ((t->t == LESS_LESS || t->t == LESS) && (in < 1))
		{
			if(t->prev)
				if((t->prev)->t == LESS)
					cmd__->is_heredoc = true;
			cmd__->input = (t->next)->cmd;
			in++;
		}
		// pipe [|] and separator [;]
		if (t->t == SEPARATOR || t->t == PIPE || t->prev == NULL)
		{
			cmd__->n_redirections = ou;
			if(!in)
				cmd__->input = (t->t == SEPARATOR || t->prev == NULL) ? "STD_IN" : "pipe";
			if(!ou)
			{
				if(lst_was_pipe)
				{
					cmd__->output = ft_strdup("pipe");
					lst_was_pipe = false;
				}
				else
					cmd__->output = ft_strdup("STD_OUT");
			}
			if(!(ft_strcmp(cmd__->output, "STD_OUT") == 0) && ft_strcmp(cmd__->output, "pipe") != 0 
					&& ft_strlen(cmd__->output) <= 1)
				fn_revstr(cmd__->output);
			if (t->t == PIPE || t->t == SEPARATOR)
			{
				lst_was_pipe = (t->t == PIPE) ? (true) : (false);
				//if(cmd__->command)
				//{
					i++;
				//}
				s_ou_count = ou = in = 0;
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
	apply_commands_reverse(t_m);
	apply_appends_reverse(t_m);
	apply_space_removal(t_m);
	apply_is_piped_out(t_m);	
	return (0);
}
