/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:23:33 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/26 18:02:56 by jsaintho         ###   ########.fr       */
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

#include <libft.h>

enum TOKEN_TYPE
{
	COMMAND,
	OPERATOR,
	PIPE,
	VARIABLE
};

typedef struct s_token
{
	char 			*v;
	enum TOKEN_TYPE	type;
	struct s_token	*prev;
	struct s_token	*next;
} token;


// TOKENIZE
void	parse_command(char *cmd, token **token_list);
token	*token_new(char *s, enum TOKEN_TYPE ty);
void	token_push(token **token_lst, token *new_t);


#endif
