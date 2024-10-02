/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:07:22 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/02 15:50:54 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char **argv, char **env)
{	
	char 	*line;
	token	**cmd_tokens;
	
	(void)argv;
	if(!env || argc > 1)
		return (0);
	line = readline("$ ");
	while(line)
	{
		cmd_tokens = (token **) malloc(sizeof(token **));
		*cmd_tokens = NULL;
		printf("---------- COMMAND-VALUE -----\n");
		printf("-> %s\n", line);
		if( parse_command(line, cmd_tokens) >= 0)
		{
			show_tokens(cmd_tokens);
			parse_to_executor(cmd_tokens);	
		}
		line = readline("$ ");
	}
	exit(EXIT_SUCCESS);
}
