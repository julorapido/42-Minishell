/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:23:33 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/03 16:51:04 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>

#include <libft.h>

/*
{
	PIPE = |
	OPERATOR = [>, <, >>]
	COMMAND = [bin, ls, cat -e, pwd]
	VARIABLE = "<: Hello World!2"
}
*/

enum TOKEN_TYPE
{
	COMMAND,
	COMMAND_FLAG,
	PIPE,
	LESS,
	LESS_LESS,
	GREAT,
	GREAT_GREAT,
	QUOTE,
	SEPARATOR,
	ARGUMENT
};

//	TOKEN
typedef struct s_token
{
	char 			*cmd;
	enum TOKEN_TYPE	t;
	struct s_token	*prev;
	struct s_token	*next;
}	token;

//	COMMAND
typedef struct s_cmd
{
	char	*command;	
	char	*input;
	char	*output;
}	t_cmd;


// TOKEN UTILITES
token	*token_new(char *s, enum TOKEN_TYPE ty);
void	token_push(token **token_lst, token *new_t);
void	show_tokens(token **cmd_tokens);
token	*token_last(token	*head);
char	*token_type_to_str(enum TOKEN_TYPE t);


// PARSING
int		parse_command(char *cmd, token **token_list);
int		parse_to_executor(token **cmd_tokens);


#endif
