/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 16:27:11 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/23 16:11:30 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// https://medium.com/@abkabex/minishell-42-f680eadde592

token	*token_last(token	*head)
{
	token	*t;

	t = head;
	while (t->next)
	{
		t = t->next;
		if (t->next == NULL)
			return (t);
	}
	return (t);
}

enum TOKEN_TYPE char_to_token(char c)
{
	if (c == '<')
		return (LESS);
	else if (c == '>')
		return (GREAT);
	else if (c == '|')
		return (PIPE);
	else if (c == ';')
		return (SEPARATOR);
	else
		return (COMMAND);
}

bool	is_char_operator(char c)
{	
	if (c == ';' || c == '<' || c == '>' || c == '|' || c == '$' 
		|| c == '.' || c == '/')
		return (true);
	else
		return (false);
}

token	*token_new(char *s, enum TOKEN_TYPE ty)
{
	token	*new_t;

	new_t = (token *) malloc(sizeof(token ));
	if (!new_t)
		return (NULL);
	new_t->cmd = s;
	//new_t->t = (enum TOKEN_TYPE *) malloc(sizeof(enum TOKEN_TYPE ));
	new_t->t = ty;
	new_t->next = NULL;
	new_t->prev = NULL;
	return (new_t);
}

void	token_push(token **token_lst, token *new_t)
{
	token	*t;

	if (*token_lst)
	{
		t = token_last(*token_lst);
		new_t->prev = t;
		t->next = new_t;	
	}
	else
	{
		*token_lst = new_t;
		(*token_lst)->next = NULL;
		(*token_lst)->prev = NULL;
	}
}

void	token_dbl_push(token **token_lst, token *new_t)
{
	token_push(token_lst, new_t);
	token_push(token_lst, new_t);
}

char	*token_type_to_str(enum TOKEN_TYPE t)
{
	char	*s;

	if(t == SEPARATOR)
		s = "(;) SEPARATOR";
	if(t == ARGUMENT)
		s = "($) ARG";
	if(t == COMMAND)
		s = "COMMAND";
	if(t == COMMAND_FLAG)
		s = "COMMAND FLAG";
	if(t == GREAT)
		s = "> OUTPUT REDIRECTION";
	if(t == LESS)
		s = "< INPUT REDIRECTION";
	if(t == GREAT_GREAT)
		s = ">> OUTPUT REDIRECTION";
	if(t == LESS_LESS)
		s = "<< INPUT REDIRECTION";
	if(t == PIPE)
		s = "| PIPE";
	if(t == QUOTE)
		s = "\"\" QUOTES";
	return (s);
}

void	show_tokens(token **cmd_tokens)
{
	token	*t;
	char	*s; 
	int		i;

	printf("---------- TOKENS ----------\n");	
	i = 0;
	t = *cmd_tokens;
	while(t)
	{
		s = token_type_to_str(t->t);
		printf(
			"- [%s] %s \n", 
			s, 
			(t->t == COMMAND || t->t == COMMAND_FLAG || t->t == QUOTE || t->t == GREAT) ? (t->cmd) : ("")
		);
		t = t->next;
		i++;
	}	
}

void	free_tokens(token **t_arr)
{
	token	*head;
	token	*t;

	head = *t_arr;
	while (head)
	{
		t = head;
		head = head->next;
		if (ft_strlen(t->cmd) > 0)
			free(t->cmd);	
		free(t);
	}
	free(t_arr);
}
