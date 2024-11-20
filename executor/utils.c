/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:48:53 by gchauvot          #+#    #+#             */
/*   Updated: 2024/11/20 15:17:28 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_c_args(t_minishell	*t_m)
{
	int	i;

	i = 0;
	while (t_m->c_args[i])
	{
		free(t_m->c_args[i]);
		i++;
	}
	free(t_m->c_args[i]);
	free(t_m->c_args);
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
		ft_putendl_fd(file, 2);
		return (ret);
	}
	return (ret);
}

char	*get_env(char **env)
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

char	*bget_path2(char *cmd, char **env)
{
	char	**b_path;
	char	**e_cmd;
	char	*exec_cmd;
	char	*str_path;
	int		i;

	i = -1;
	b_path = ft_split(get_env(env), ':');
	e_cmd = cmd;
	while (b_path[++i])
	{
		str_path = ft_strjoin(b_path[i], "/");
		// exec_cmd = ft_strjoin(str_path, e_cmd[0]);
		exec_cmd = ft_strjoin(str_path, e_cmd);
		free(str_path);
		if (access(exec_cmd, F_OK | X_OK) == 0)
		{
			//ft_free_tab(e_cmd);
			return (exec_cmd);
		}
		free(exec_cmd);
	}
	//ft_free_tab(e_cmd);
	ft_free_tab(b_path);
	return (cmd);
}

void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
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
