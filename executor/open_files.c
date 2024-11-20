/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:49:14 by gchauvot          #+#    #+#             */
/*   Updated: 2024/11/20 14:37:50 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dupclose(int fd2, int fd1)
{
	if (dup2(fd2, fd1) == -1)
		return (perror("minishell: "), exit(126), 1);
	if (close(fd2) == -1)
		return (perror("minishell: "), exit(126), 1);
	return (0);
}

char	*tmpnamer(void)
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
int gbs=0;
void	handler2(int signum)
{	
	if (signum == SIGINT)
	{
		rl_done = 1;
		gbs = 1;
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();

	}
	return ;
}

int	heredoc(char *eof, t_cmd *cmd)
{
	char	*tempfile;
	pid_t	pid;
	char	*line;
	int		docfd;
	int		tmfr=0;

	tempfile = tmpnamer();
	if (!tempfile)
		return (perror("join err: "), -1);
	signalignore(SIGINT);
	pid = fork();
	if (!pid)
	{
		signalsetter(SIGINT, SIG_DFL);
		docfd = open(tempfile, O_WRONLY | O_CREAT | O_TRUNC, 777);
		if (docfd == -1)
			return (perror("openhere err: "), -1);
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
			else
			{
				//fprintf(stderr, "!line\n");
				continue ;
			}	
		}
		free(line);
		close(docfd);
		exit (1);
	}
	if (pid)
		waitpid(pid, &tmfr, 0);
	signalsetter(SIGINT, handler);
	//fprintf(stderr, "tmfr= &%d&\n", tmfr);
	if (tmfr == 2)
		return (cmd->input = tempfile, -1);
	cmd->input = tempfile;
	return (3);
}

int	heredocalloc(t_minishell *t_m)
{
	t_cmd	*c;
	size_t	i;

	i = 0;
	while (i <= t_m->cmd_count - 1)
	{
		c = &(t_m->commands[i]);
		if (c->is_heredoc)
			if(heredoc(c->input, c)==-1)
				return -1;
		i++;
	}
	//write(2,"\n",1);
	return (1);
}

int	delete_heredocs(t_minishell *t_m)
{
	size_t	i;
	t_cmd	*c;

	i = 0;
	while (i < t_m->cmd_count)
	{
		c = &(t_m->commands[i]);
		if (c && c->is_heredoc)
		{
			if (c->input)
			{
				if (access(c->input, F_OK) == 0)
				{
					if (unlink(c->input) == -1)
						perror("unlink heredoc: ");
				}
			}
		}
		i++;
	}
	return (0);
}
