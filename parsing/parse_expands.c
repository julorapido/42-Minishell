/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:18:44 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/28 17:51:12 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**eq_split(char *s)
{
	char	**r;
	int		i;
	int		j;

	r = (char **) malloc(2 * sizeof(char *));
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	r[0] = (char *) malloc(i * sizeof(char));
	r[1] = (char *) malloc((ft_strlen(s) - (i + 1)) * sizeof(char));
	i = 0;
	while (s[i] && s[i] != '=')
	{
		r[0][i] = s[i];
		i++;
	}
	r[0][i] = '\0';
	j = i + 1;
	i = 0;
	while (s[j + i] != '\0')
	{
		r[1][i] = s[j + i];
		i++;
	}
	r[1][i] = '\0';
	return (r);
}

static void print_expands(t_minishell *t)
{
	int	i;

	i = 0;
	if (!t->expands[i].key)
		return ;
	printf("================ EXPANDS [$] =================\n");
	while (t->expands[i].key)
	{
		printf("> $%s= %s \n", t->expands[i].key, t->expands[i].value);
		i++;
	}
}

static void	modify_cmd(t_expand e, t_cmd *cmd, char *s, int ix)
{
	int i;
	int	k;
	int	j;

	free(cmd->command);
	cmd->command = (char *) malloc((ft_strlen(s) - (ft_strlen(e.key) + 1) + (ft_strlen(e.value)) + 1) * sizeof(char));
	i = 0;
	k = 0;
	j = 0;	
	while (s[k])
	{
		if (i == ix)
		{
			while (e.value[j])
			{
				cmd->command[i] = e.value[j];
				j++;	
				i++;
			}
			k += ft_strlen(e.key) + 1;
		}
		else
		{
			cmd->command[i] = s[k];
			k++;
			i++;
		}
	}
	cmd->command[i] = '\0';
}

static void	apply_expands(t_minishell *t, int i, int i2)
{
	int		e;
	char	*s;
	char	*sub_s;

	while ((size_t)(i) < t->cmd_count && (size_t)(i) < i2)
	{	
		s = (&(t->commands[i]))->command;
		e = 0;
		if (ft_strchr(t->commands[i].command, '$'))
		{	
			while (t->expands[e].key)
			{
				sub_s = ft_substr(s, ft_m_strchr_i(s, '$', '$') + 1, ft_strlen(t->expands[e].key));	
				if (ft_strcmp(sub_s, t->expands[e].key) == 0)
					modify_cmd(t->expands[e], &(t->commands[i]), ft_strdup(s), ft_m_strchr_i(s, '$', '$'));
				free(sub_s);
				e++;
			}
		}
		i++;
	}
}

static int	key_in_expand(t_minishell *t, char *key_)
{
	int	i;

	i = 0;
	while (t->expands[i].key)
	{
		if(ft_strcmp(t->expands[i].key, key_) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static int	next_expand(t_minishell *t, int i)
{
	char *s_;

	while ((size_t)(i) < t->cmd_count)
	{
		s_ = (&(t->commands[i]))->command;	
		if (!ft_strchr(s_, '\"') && ft_strchr(s_, '='))
			return (i);
		i++;
	}
}

void	parse_expands(t_minishell *t)
{
	int		i;
	int		j;
	int		x;
	char	*s_;
	char	**spl_;

	i = 0;
	j = 0;
	while ((size_t)(i) < t->cmd_count)
	{
		s_ = (&(t->commands[i]))->command;	
		if (!ft_strchr(s_, '\"') && ft_strchr(s_, '='))
		{
			(&(t->commands[i]))->continue_ = true;	
			spl_ = eq_split(s_);
			x = j;
			if (key_in_expand(t, spl_[0]) != -1)
			{
				x = key_in_expand(t, spl_[0]);
			}
			// printf("expand[%d] = {%s : %s} \n", x, spl_[0], spl_[1]);
			(t->expands[x]).key = spl_[0];
			(t->expands[x]).value = spl_[1];
			if (key_in_expand(t, spl_[0]) == -1)
				j++;
		}	
		apply_expands(t, i, next_expand(t, i));
		i++;
	}	
	// print_expands(t);
}
