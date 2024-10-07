/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:07:22 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/07 14:18:35 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>

void	minishell(void)
{
	char 	*line;
	token	**cmd_tokens;

	line = readline("$ ");
	while(line)
	{
		if(ft_strlen(line) < 2)
		{
			line = readline("$ ");
			continue;
		}
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
}

int main(int argc, char **argv, char **env)
{	
	t_minishell	*t_m;	
	
	(void)argv;
	if(!env || argc > 1)
		return (0);
	t_m = (t_minishell *) malloc(sizeof(t_minishell));
	if(!t_m)
		return (EXIT_FAILURE);
	env_init(t_m, env);
	minishell();
	exit(EXIT_SUCCESS);
}
