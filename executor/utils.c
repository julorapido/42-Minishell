/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:48:53 by gchauvot          #+#    #+#             */
/*   Updated: 2024/12/06 15:47:29 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env(char **env)
{
	int		i;
	int		j;
	char	*s;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		s = ft_substr(env[i], 0, j);
		if (ft_strcmp(s, &("PATH"[0])) == 0)
		{
			free(s);
			return (env[i] + j + 1);
		}
		free(s);
		i++;
	}
	return (NULL);
}

int	open_file(char *file, int in_out, int append)
{
	int	ret;

	if (in_out == 0)
		ret = open(file, O_RDONLY, 0644);
	if (in_out == 1 && !append)
		ret = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (in_out == 1 && append)
		ret = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (ret == -1)
	{
		perror("minishell open_file: ");
		return (ret);
	}
	return (ret);
}

t_file	*last_file(int _out, t_cmd *c)
{
	int		i;
	t_file	*f;

	i = 0;
	while (i < c->f_i)
	{
		if (_out)
		{
			if (c->files[i]._out)
				f = &(c->files[i]);
		}
		else
		{
			if (!c->files[i]._out)
				f = &(c->files[i]);
		}
		i++;
	}
	return (f);
}

char	*bget_path2(char *cmd, char **env)
{
	char	**b_path;
	char	**e_cmd;
	char	*exec_cmd;
	char	*str_path;
	int		i;

	i = -1;
	b_path = ft_split_quotes(get_env(env), ':', 0);
	e_cmd = ft_split_quotes(cmd, ' ', 0);
	if (!b_path || !e_cmd)
		return (NULL);
	while (b_path[++i])
	{
		str_path = ft_strjoin(b_path[i], "/");
		exec_cmd = ft_strjoin(str_path, e_cmd[0]);
		free(str_path);
		if (access(exec_cmd, F_OK | X_OK) == 0)
		{
			return (exec_cmd);
		}
		free(exec_cmd);
	}
	ft_free_tab(e_cmd);
	ft_free_tab(b_path);
	return (cmd);
}

char	**pipe_env(t_minishell *t_m)
{
	int		i;
	int		x;
	char	**env;
	t_env	*temp;

	i = 0;
	x = 0;
	temp = t_m->env;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	env = ft_calloc(i + 1, sizeof(char *));
	if (!env)
		return (NULL);
	temp = t_m->env;
	while (x < i)
	{
		env[x] = temp->value;
		temp = temp->next;
		x++;
	}
	return (env);
}
