/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 16:27:11 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/02 14:24:36 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// https://medium.com/@abkabex/minishell-42-f680eadde592

static token	*token_last(token	*head)
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


token	*token_new(char *s, enum TOKEN_TYPE ty)
{
	token	*new_t;

	new_t = (token *) malloc(sizeof(token ));
	if (!new_t)
		return (NULL);
	new_t->cmd = s;
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
		t->next = new_t;
		new_t->next = NULL;
	}
	else
	{
		*token_lst = new_t;
		(*token_lst)->next = NULL;
	}
}

void	show_tokens(token **cmd_tokens)
{
	token	*t;
	char	*s; 
	int		i;

	printf("==================================================\n");
	printf("=================       TOKENS        ============\n");	
	i = 0;
	t = *cmd_tokens;
	while(t)
	{
		if(t->t == ARGUMENT)
			s = "[$] ARG";
		if(t->t == COMMAND)
			s = "COMMAND";
		if(t->t == COMMAND_FLAG)
			s = "COMMAND FLAG";
		if(t->t == GREAT)
			s = "[>] OUTPUT REDIRECTION";
		if(t->t == LESS)
			s = "[<] INPUT REDIRECTION";
		if(t->t == GREAT_GREAT)
			s = "[>>] OUTPUT REDIRECTION";
		if(t->t == LESS_LESS)
			s = "[<<] INPUT REDIRECTION";
		if(t->t == PIPE)
			s = "[|] PIPE";
		if(t->t == QUOTE)
			s = "[\"\"] QUOTE VAR";
		printf(
			"- [%s] %s \n", 
			s, ((t->t == COMMAND || t->t == COMMAND_FLAG || t->t == QUOTE) ? (t->cmd) : (""))
		);
		t = t->next;
		i++;
	}
	printf("==================================================\n");
}
