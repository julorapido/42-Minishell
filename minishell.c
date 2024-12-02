/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:07:22 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/02 17:19:37 by julessainthor    ###   ########.fr       */
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
		t_m->set = ft_strdup("><");
		if(ft_strlen(line) < 1)
		{
			line = readline("$ ");
			add_history(line);
			continue ;
		}
		t_mltsplit *s = ft_multisplit(line, " ");
		if (parse_errors(s, line) != '\0')
		{
			printf("zsh: parse error near `%c' \n", parse_errors(s, line));
			free_multisplit(s);
			line = readline("$ ");
			add_history(line);
			continue ;
		}
		fdp_parsing(line, t_m);
		exec_cmds(t_m);
		free_multisplit(s);
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
