/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:55:36 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/22 15:16:25 by jsaintho         ###   ########.fr       */
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
	if (!up_s || up_s == NULL || ft_strlen(up_s) <= 1)
		return ;
	s_p = ft_split(up_s, ' ');
	while (s_p[i])
		i++;
	i--;
	while (i >= 0)
	{
		s = s_p[i];
		j = 0;
		while (s[j] != '\0')
		{
			up_s[k] = s[j];
			j++;
			k++;
		}
		up_s[k] = ' ';
		k++;
		i--;
	}
	up_s[k - 1] = '\0';
	i = 0;
	while (s_p[i])
	{
		free(s_p[i]);
		i++;
	}
}

char	*cmd_remove_lstspace(char *s)
{
	int		i;
	int		j;
	int		l;
	char	*new_s;

	if (!s || s[0] == '\0' || !(*s))
		return (NULL);
	i = 0;
	l = ft_strlen(s);
	if (s[l - 1] == ' ')
		i = 1;
	new_s = (char *) malloc(((l - i) + 1) * sizeof(char));
	if (!new_s)
		return (NULL);
	j = 0;
	while (j < l - i)
	{
		new_s[j] = s[j];
		j++;
	}
	new_s[j] = '\0';
	return (new_s);
}

void	apply_space_removal(t_minishell *t_m)
{
	int		i;
	t_cmd	*cmd__;
	char	*temp_cmd;

	i = 0;
	while ((size_t)(i) < t_m->cmd_count)
	{
		cmd__ = &(t_m->commands[i]);
		if (!(cmd__->command))
			break;
		temp_cmd = cmd_remove_lstspace(cmd__->command);
		cmd__->command = temp_cmd;
		i++;
	}
}

void	apply_is_piped_out(t_minishell *t_m)
{
	int		i;
	t_cmd	*cmd__;

	i = 0;
	while ((size_t)(i) < t_m->cmd_count)
	{
		if(!(&(t_m->commands[i])))
			break ;
		cmd__ = &(t_m->commands[i]);
		if (&(t_m->commands[i + 1]) && ((&(t_m->commands[i + 1]))->input))
		{
			//if ((cmd__->n_redirections > 0) && (ft_strcmp((&(t_m->commands[i + 1]))->input , "pipe") == 0) )
				// cmd__->is_piped_out = true;
		}
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
		if(!(&(t_m->commands[i])))
			break ;
		cmd__ = &(t_m->commands[i]);
		printf("-Command %d [cmd: %s| in: %s%s| out: %s]", i,
				cmd__->command,
				cmd__->is_heredoc ? "(heredoc <<) ": "",
				cmd__->input, cmd__->output
		);
		if(&(t_m->commands[i + 1]) && ((&(t_m->commands[i + 1]))->input))
		{
			if((cmd__->n_redirections > 0) && (ft_strcmp((&(t_m->commands[i + 1]))->input , "pipe") == 0) )
				printf(" [piped out |] ");
		}
		if (cmd__->n_redirections > 1)
		{
			printf(" [%d out_files : {", cmd__->n_redirections);
			for(int i = 0; i < cmd__->n_redirections; i++)
				printf("%s%s", cmd__->appends[i] == 1 ? ">>": ">", i < cmd__->n_redirections - 1 ? ", "  : "");
			printf("} ]");
		}else{
			if(cmd__->appends[0] == 1)
				printf(" [>>]");
		}
		printf("\n");
		i++;
	}
}

void	apply_commands_reverse(t_minishell	*t_m)
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

void	apply_appends_reverse(t_minishell *t_m)
{
	/*int		i;
	int		j;
	int		temp;

	i = 0;
	while ((size_t)(i) < t_m->cmd_count)
	{
		j = 0;
		while(j < t_m->commands[i]->n_redirections)
		{
			temp = t_m->commands[i]->appends[j];
			t_m->commands[i] = ((t_m->commands[i])->appends)[t_m->commands[i]->n_redirections - j];
			((t_m->commands[i]->)appends)[t_m->commands[i]->n_redirections - j] = temp;
			j++;
		}
		i++;
	}*/
}
