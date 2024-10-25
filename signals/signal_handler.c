/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:43:14 by gchauvot          #+#    #+#             */
/*   Updated: 2024/10/25 14:22:38 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signalsetter(int signum, void (*handler)(int))
{
	struct sigaction act;
	//struct __sigset_t jo;
	
	ft_bzero(&act, sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = handler;
	//sigaddset(&act.sa_mask, SIGINT);
	sigaction(signum, &act, 0);
}

void	signalignore(int signum)
{
	struct sigaction act;
	//struct __sigset_t jo;
	
	ft_bzero(&act, sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = SIG_IGN;
	//sigaddset(&act.sa_mask, SIGINT);
	sigaction(signum, &act, 0);
}

void handler(int signum)
{	
	if(signum == SIGINT)
	{
		write(2, "\n",1);
		rl_replace_line("",0);
		rl_on_new_line();
		rl_redisplay();
	}
	return ;
}

void	int_heredoc(int signum)
{
	(void)(signum);
	exit(1);
}
void shlvldetector(t_env *env)
{
	t_env *temp;

	temp = env;
	while(temp)
	{
		//fprintf(stderr,"tenvvalue: %s\n",temp->value);
		if (ft_strlen(temp->value) == 7)
		{
			if(!ft_strncmp(temp->value, "SHLVL=1", 7))
			{
				//fprintf(stderr,"joe\n");
				//signalsetter();
				// global_stat = 0;
			}
			// else
			// 	global_stat= 1;
		}
		temp = temp->next;
	}
}
