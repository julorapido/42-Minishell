/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:07:22 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/24 15:09:01 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>

void	minishell(t_minishell *t_m)
{
	char 	*line;
	// token	**cmd_tokens;
	signalignore(SIGQUIT);
	signalsetter(SIGINT, handler);
	
	line = readline("$ ");
	add_history(line);
	while(line)
	{
		if(ft_strlen(line) < 1)
		{
			line = readline("$ ");
			add_history(line);
			continue ;
		}
		t_m->cmd_tokens = (token **) malloc(sizeof(token **));
		*(t_m->cmd_tokens) = NULL;
		
		parse_errors(line, t_m);
		// free(t_m->splt_cat);
		if(t_m->parse_error)
		{
			printf("zsh: parse error near `%c%c' \n", t_m->e_v[0], t_m->e_v[1]);
			line = readline("$ ");
			add_history(line);
			continue ;
		}
		parse_tokens(line, t_m->cmd_tokens, t_m);
		//show_tokens(t_m->cmd_tokens);
		parse_commands(t_m, t_m->cmd_tokens);
		print_commands(t_m);
		//exec_cmds(t_m);
		parse_free(t_m);

		line = readline("$ ");
		add_history(line);
	}
}

int main(int argc, char **argv, char **env)
{	
	t_minishell	*t_m;	
	
	(void)argv;
	if(!env || argc > 1)
		return (0);
	shlvlhandler(env);
	t_m = (t_minishell *) malloc(sizeof(t_minishell));
	if(!t_m)
		return (EXIT_FAILURE);
	env_init(t_m, env);
	minishell(t_m);
	exit(EXIT_SUCCESS);
}
