/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:46:03 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/26 18:05:51 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static enum TOKEN_TYPE	switcher(char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		//if()
		i++;
	}
	return (OPERATOR);
}


void	parse_command(char *cmd, token **cmd_tokens)
{
	char	**parsed_cmd;
	token	*t;
	int		i;

	parsed_cmd = ft_split(cmd, ' ');
	i = 0;
	while (parsed_cmd[i])
	{
		t = token_new(
			parsed_cmd[i],
			switcher(cmd[i])
		);
		token_push(cmd_tokens, t);
		i++;
	}
}

