/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:07:22 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/29 11:48:34 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>
int sigcatch = 0;

void	minishell(t_minishell *t_m)
{
	char 	*line;
	signalignore(SIGQUIT);
	signalsetter(SIGINT, handler);
	t_m->exstat = 0;
	t_m->exp_starter = 0;
	
	line = readline("$ ");
	add_history(line);
	while(line)
	{
		t_m->exp_starter = 0;
		if(ft_strlen(line) < 1)
		{
			line = readline("$ ");
			add_history(line);
			continue ;
		}
		/*
		if (quote_errors(line) != -1 || parse_errors(ft_multisplit(line, " ")) != '\0')
		{
			printf("zsh: parse error near `' \n");
			line = readline("$ ");
			add_history(line);
			continue ;
		}
		*/
		fdp_parsing(line, t_m);
		//parse_free(t_m);
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
	t_m = ft_calloc(1, sizeof(t_minishell));
	if(!t_m)
		return (EXIT_FAILURE);
	env_init(t_m, env);
	minishell(t_m);
	exit(EXIT_SUCCESS);
}
