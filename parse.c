/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:46:03 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/26 18:13:35 by jsaintho         ###   ########.fr       */
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


static char *space_line(char *cmd_line)
{
	char *new;
	int  i;
	int  j;

	i = 0;
	j = 0;
	new = space_alloc(line);
	while (new && line[i])
	{
		if (quotes(line, i) != 2 && line[i] == '$' && i && line[i - 1] != '\\')
		new[j++] = (char)(-line[i++]);
		else if (quotes(line, i) == 0 && is_sep(line, i))
		{
	 		new[j++] = ' ';
			new[j++] = line[i++];
			if (quotes(line, i) == 0 && line[i] == '>')
				new[j++] = line[i++];
				new[j++] = ' ';
		}
		else
			new[j++] = line[i++];
	 }
	 new[j] = '\0';
 	_memdel(line);
 	return (new);
}

void	parse_command(char *cmd, token **cmd_tokens)
{
	char	*spaced_cmd;
	char	**split_cmd;
	token	*t;
	int		i;

	spaced_cmd = space_line(cmd);
	split_cmd = ft_split(spaced_cmd, ' ');
	i = 0;
	while (split_cmd[i])
	{
		t = token_new(
			split_cmd[i],
			switcher(cmd[i])
		);
		token_push(cmd_tokens, t);
		i++;
	}
}

