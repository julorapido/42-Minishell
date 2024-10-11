/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:07:22 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/11 17:20:26 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>

void	minishell(t_minishell *t_m)
{
	char 	*line;
	token	**cmd_tokens;

	line = readline("$ ");
	while(line)
	{
		if(ft_strlen(line) < 1)
		{
			line = readline("$ ");
			continue ;
		}
		cmd_tokens = (token **) malloc(sizeof(token **));
		*cmd_tokens = NULL;
		parse_tokens(line, cmd_tokens, t_m);
		if(t_m->parse_error)
		{
			printf("parse error near `%s' \n", t_m->parse_error_value);
			line = readline("$ ");
			continue ;
		}
		show_tokens(cmd_tokens);
		parse_commands(t_m, cmd_tokens);
		exec_cmds(t_m);

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
	minishell(t_m);
	exit(EXIT_SUCCESS);
}
