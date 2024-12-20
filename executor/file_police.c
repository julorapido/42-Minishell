/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_police.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:41:20 by gchauvot          #+#    #+#             */
/*   Updated: 2024/12/09 13:29:17 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_exec2(char *cmd, char **env)
{
	char	**ft_cmd;
	char	*path;

	if (ft_stronly(cmd, ' '))
		return (0);
	ft_cmd = ft_split_quotes(cmd, ' ', 0);
	path = bget_path2(ft_cmd[0], env);
	ft_apply_rmquote(ft_cmd);
	if (execve(path, ft_cmd, env) == -1)
	{
		ft_putstr_fd("solo: command not found: ", STDERR_FILENO);
		ft_putendl_fd(ft_cmd[0], 2);
		ft_free_tab(ft_cmd);
		exit(127);
	}
	return (0);
}

static int	dupclose(int fd2, int fd1)
{
	if (dup2(fd2, fd1) == -1)
		return (perror("minishell: dup2: "), exit(126), 1);
	if (close(fd2) == -1)
		return (perror("minishell: close: "), exit(126), 1);
	return (0);
}

static int	closepipesonfail(t_minishell *t_m, int i)
{
	close(t_m->pipes_fd[i][0]);
	close(t_m->pipes_fd[i][1]);
	if (i > 0)
	{
		close(t_m->pipes_fd[i - 1][0]);
		close(t_m->pipes_fd[i - 1][1]);
	}
	exit(1);
}

static int	child_handler(t_minishell *t_m, size_t i, t_cmd *c)
{
	int	fd;
	int	a;

	a = 0;
	while (a < c->f_i)
	{
		fd = open_file(c->files[a].f_name,
				c->files[a]._out, c->files[a].append);
		if (fd == -1)
			return (1);
		dupclose(fd, c->files[a]._out);
		a++;
	}
	if (i > 0 && c->n_in == 0)
	{
		dupclose(t_m->pipes_fd[i - 1][0], 0);
		close(t_m->pipes_fd[i - 1][1]);
	}
	close(t_m->pipes_fd[i][0]);
	if (c->n_out == 0 && i < t_m->cmd_count - 1)
		dupclose(t_m->pipes_fd[i][1], 1);
	else
		close(t_m->pipes_fd[i][1]);
	return (0);
}

int	child_molestor(t_minishell *t_m, t_cmd *c, size_t i, int c_int, char **nenv)
{
	int	exit_status;

	exit_status = 0;
	signalsetter(SIGINT, SIG_DFL);
	if (child_handler(t_m, i, c))
		closepipesonfail(t_m, i);
	if (i > 0 && c->n_in > 0)
	{
		close(t_m->pipes_fd[i - 1][0]);
		close(t_m->pipes_fd[i - 1][1]);
	}
	if (c_int != -1)
		exit_status = run_builtin(t_m, c_int, 1, c);
	else if (c->command)
		exit_status = ft_exec2(c->command, nenv);
	exit(exit_status);
}
