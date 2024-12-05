/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:49:14 by gchauvot          #+#    #+#             */
/*   Updated: 2024/12/05 12:29:35 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*tmpnamer(void)
{
	size_t	i;
	char	*res;

	i = 0;
	while (i < 10000)
	{
		res = ft_itoa(i);
		if (access(res, F_OK))
			break ;
		else
			free(res);
		i++;
	}
	return (res);
}

static int	heredoc_writer(char *eof, char *tempfile)
{
	char	*line;
	int		docfd;

	signalsetter(SIGINT, SIG_DFL);
	docfd = open(tempfile, O_WRONLY | O_CREAT | O_TRUNC, 777);
	if (docfd == -1)
		return (perror("openheredoc err: "), -1);
	while (1)
	{
		line = readline("heredoc>");
		if (line)
		{
			if (ft_strlen(eof) == ft_strlen(line))
			{
				if (!ft_strncmp(eof, line, ft_strlen(eof)))
					break ;
			}
			ft_putendl_fd(line, docfd);
			free(line);
		}
	}
	free(line);
	close(docfd);
	exit (1);
	return (1);
}

static int	heredoc(t_file *f)
{
	char	*tempfile;
	pid_t	pid;
	int		tmfr;

	tmfr = 0;
	tempfile = tmpnamer();
	if (!tempfile)
		return (perror("join err: "), -1);
	signalignore(SIGINT);
	pid = fork();
	if (!pid)
		heredoc_writer(f->f_name, tempfile);
	if (pid)
		waitpid(pid, &tmfr, 0);
	signalsetter(SIGINT, handler);
	if (tmfr == 2)
		return (f->f_name = tempfile, -1);
	f->f_name = tempfile;
	return (3);
}

int	heredocalloc(t_minishell *t_m)
{
	t_cmd	*c;
	size_t	i;
	int		a;

	i = 0;
	while (i <= t_m->cmd_count - 1)
	{
		c = &(t_m->cmds[i]);
		a = 0;
		while (a < c->f_i)
		{
			if ((c->files[a]).heredoc)
				if (heredoc(&(c->files[a])) == -1)
					return (-1);
			a++;
		}
		i++;
	}
	return (1);
}

int	delete_heredocs(t_minishell *t_m)
{
	size_t	i;
	t_cmd	*c;
	int		a;

	i = 0;
	while (i < t_m->cmd_count)
	{
		c = &(t_m->cmds[i]);
		a = 0;
		while (a < c->f_i)
		{
			if ((c->files[a]).heredoc)
			{
				if (access((c->files[a]).f_name, F_OK) == 0)
					if (unlink((c->files[a]).f_name) == -1)
						perror("unlink heredoc: ");
			}
			a++;
		}
		i++;
	}
	return (0);
}
