/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:26:30 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/28 17:45:12 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reset(t_minishell *t_m)
{
	(t_m->p_cmd).in = 0;
	(t_m->p_cmd).ou = 0;
	(t_m->p_cmd).append = 0;
}

static void	parse_commands4(token *t, t_cmd *cmd__, t_minishell *t_m)
{	
	if (t->t == GREAT_GREAT || t->t == GREAT)
	{
		if ((t->next) && (t->next)->cmd && (ft_strlen((t->next)->cmd) > 0))
		{
			if((t_m->p_cmd).ou > 0)
				cmd__->output = fn_realloc_strcat(cmd__->output, (t->next)->cmd, 1);
			else
				cmd__->output = (t->next)->cmd;
			(t_m->p_cmd).ou++;
		}
	}
	if (!(t->t == GREAT_GREAT || t->t == GREAT))	
		(t_m->p_cmd).append = 0;	
	if ((t->t == LESS_LESS || t->t == LESS) && ((t_m->p_cmd).in < 1))
	{
		if (t->prev)
			if ((t->prev)->t == LESS)
				cmd__->is_heredoc = true;
		cmd__->input = (t->next)->cmd;
		(t_m->p_cmd).in++;
	}
}

static void	parse_commands3(t_cmd *cmd__, token *t, t_minishell *t_m, int *i, bool *lst_was_pipe)
{
	cmd__->n_redirections = (t_m->p_cmd).ou;
	if (!((t_m->p_cmd).in))
		cmd__->input = M_CMD(t->t == SEPARATOR || t->prev == NULL);
	if (!((t_m->p_cmd).ou))
	{
		if ((*lst_was_pipe))
		{
			cmd__->output = ft_strdup("pipe");
			(*lst_was_pipe) = false;
		}
		else
			cmd__->output = ft_strdup("STD_OUT");
	}
	if (!(ft_strcmp(cmd__->output, "STD_OUT") == 0) && ft_strcmp(cmd__->output, "pipe") != 0 && ft_strlen(cmd__->output) <= 1)
		fn_revstr(cmd__->output);
	if (t->t == PIPE || t->t == SEPARATOR)
	{
		(*lst_was_pipe) = (t->t == PIPE) ? (true) : (false);
		if ((t_m->p_cmd).append == 2)
			cmd__->is_append = true;
		(*i)++;
		reset(t_m);
	}
	fn_revstr(cmd__->command);
}

static void	parse_commands2(t_cmd *cmd__, token *t, t_minishell *t_m)
{
	if ((t->t == COMMAND || t->t == COMMAND_FLAG) || t->t == QUOTE)
	{
		if (t->prev)
			if ((t->prev)->t == GREAT || (t->prev)->t == GREAT_GREAT || (t->prev)->t == LESS || (t->prev)->t == LESS_LESS)
				(t_m->p_cmd).s = true;
		if (!((t_m->p_cmd).s))
		{
			if (!(cmd__->command))
			{
				cmd__->command = (char *) malloc((ft_strlen(t->cmd) + 1) * sizeof(char));
				ft_strlcpy(cmd__->command, t->cmd, ft_strlen(t->cmd) + 1);
			}
			else
				cmd__->command = fn_realloc_strcat(cmd__->command, t->cmd, 1);	
		}
	}
}

// PARSE [COMMANDS]
int parse_commands(t_minishell *t_m, token **cmd_tokens)
{
	token	*t;
	t_cmd	*commands;
	int		i;
	bool 	lst_was_pipe;

	i = 0;
	(t_m->p_cmd).s = false;
	t = token_last(*cmd_tokens);
	reset(t_m);
	commands = (t_cmd *) malloc(sizeof(struct s_cmd) * MAX_CMDS);
	for(int i = 0; i < MAX_CMDS; i++)
	{
		commands[i].input = commands[i].output = commands[i].command;
	}
	while (t)
	{
		parse_commands2(&commands[i], t, t_m);
		parse_commands4(t, &commands[i], t_m);
		if (t->t == SEPARATOR || t->t == PIPE || t->prev == NULL)
			parse_commands3(&commands[i], t, t_m, &i, &lst_was_pipe);
		(t_m->p_cmd).s = false;
		t = t->prev;
	}
	if (i == 0)
		(&commands[0])->n_redirections = (t_m->p_cmd).ou;
	t_m->commands = commands;
	t_m->cmd_count = i + 1;
	apply_commands_reverse(t_m);
	apply_is_stds(t_m);
	parse_expands(t_m);
	return (0);
}
