/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:55:36 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/10 17:17:30 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	fn_revstr(char *up_s)
{
	char	**s_p;
	char	*s;
	int 	i;
	int		j;
 	int		k;

	k = 0;
	i = 0;
	if (!up_s || up_s == NULL || ft_strlen(up_s) == 1)
		return ;
	s_p = ft_split(up_s, ' ');
	while (s_p[i])
		i++;
	i--;
	while (i >= 0)
	{
		s = s_p[i];
		j = 0;
		while(s[j])
		{	
			up_s[k] = s[j];
			j++;
			k++;
		}
		up_s[k] = ' ';
		k++;
		i--;	

	}
	up_s[k] = '\0';
	i = -1;
	while (s_p[i++])
		free(s_p[i]);	
}

char	*cmd_remove_lstspace(char *s)
{
	int		i;
	int		j;
	char	*new_s;

	i = 0;
	while(s[i])
		i++;
	if(s[i - 1] == ' ')
		i--;
	new_s = (char *) malloc(i * sizeof(char));
	if (!new_s)
		return (NULL);
	j = 0;
	while(j < i)
	{
		new_s[j] = s[j];
		j++;
	}
	new_s[j] = '\0';
	return (new_s);
}

void	appyl_space_removal(t_minishell *t_m)
{
	int		i;
	t_cmd	*cmd__;

	i = 0;
	while ((size_t)(i) < t_m->cmd_count)
	{
		cmd__ = &(t_m->commands[i]);
		if (!(cmd__->command))
			break;
		cmd__->command = cmd_remove_lstspace(cmd__->command);
		i++;
	}
}

void	print_commands(t_minishell *t_m)
{
	int		i;
	t_cmd	*cmd__;

	i = 0;
	printf("---------- EXECUTOR-COMMANDS ----------\n");
	while((size_t)(i) < t_m->cmd_count)
	{
		cmd__ = &(t_m->commands[i]);	
		printf("-Command %d [cmd: %s| in: %s| out: %s| outfiles_n %d] \n", i, cmd__->command, cmd__->input, cmd__->output, cmd__->n_redirections);
		i++;
	}
}

void	rev_tm_commands(t_minishell	*t_m)
{
	int		i;
	t_cmd	temp;

	i = 0;
	while ((size_t)(i) < t_m->cmd_count / 2)
	{
		temp = t_m->commands[i];

		t_m->commands[i] = t_m->commands[(t_m->cmd_count - 1) - i];
		t_m->commands[(t_m->cmd_count - 1) - i] = temp;
		i++;
	}
}
