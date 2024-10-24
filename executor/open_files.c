/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:49:14 by gchauvot          #+#    #+#             */
/*   Updated: 2024/10/24 12:03:23 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dupclose(int fd2, int fd1)
{
	if (dup2(fd2, fd1) == -1)
		return (perror("minishell: "), exit(1), 1);
	if (close(fd2) == -1)
		return (perror("minishell: "), exit(1), 1);
	return (0);
	
}

char *tmpnamer()
{
	size_t		i;
	char *res;

	i = 0;
	//fprintf(stderr, "name: %s\n", name);
	//chdir("/home/gchauvot/Proj/Minishells/v7/temp/");
	// name[0]='/';
	// // if (name[0]=='a')
	// // 	fprintf(stderr, "namqweqwee: %s\n", name);
	// name[1]='a';
	// name[2]=0;
	while (i < 5000)
	{
		res = ft_itoa(i);
		if(access(res, F_OK))
			break ;
		else
			free(res);
		i++;
	}
	return (res);
}

int	heredoc(char *eof, t_cmd *cmd, t_minishell *t_m)
{

	char	*tempfile;


	tempfile = tmpnamer();
	if (!tempfile)
		return (perror("join err: "), -1);
	pid_t pid;
	signalignore(SIGINT);
	int fdin;
	fdin=dup(0);
	close(0);
	pid = fork();
	if (!pid)
	{
		dup2(fdin, 0);
		close(fdin);
		char	*line;
		int		docfd;
		signalsetter(SIGINT, SIG_DFL);
		docfd = open(tempfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if(docfd == -1)
			return (perror("openhere err: "), -1);
		while (1)
		{
			line = readline("heredoc>");
			if (line)
			{
				if(ft_strlen(eof) == ft_strlen(line))
				{
					if(!ft_strncmp(eof,line, ft_strlen(eof)))
						break ;
				}
				ft_putendl_fd(line, docfd);
				free(line);
			}			
		}
		if (line)
			free(line);
		close(docfd);
		exit (0);
	}
	if(pid)
	{
		waitpid(pid,0,0);
		dup2(fdin, 0);
		rl_replace_line("",0);
	}
	signalsetter(SIGINT, handler);
	free(cmd->input);
	cmd->input = tempfile;
	return(3);
}

int	heredocalloc(t_minishell *t_m)
{
	t_cmd	*c;
	size_t	i;

	i = 0;
	// t_m->heredocs = ft_calloc(t_m->cmd_count + 1, sizeof(int));
	// if (!t_m->heredocs)
	// 	return (perror ("minishell_heredocalloc:"), -1);
	while(i <= t_m->cmd_count - 1)
	{
		c = &(t_m->commands[i]);
		if(c->is_heredoc)
			heredoc(c->input, c, t_m);
		// if(t_m->heredocs[i] == -1)
		// 	return (-1);
		i++;

	}
	return (1);
}

int	delete_heredocs(t_minishell *t_m)
{
	size_t	i;
	t_cmd	*c;

	i = 0;
	while(i < t_m->cmd_count)
	{
		c = &(t_m->commands[i]);
		if(c->is_heredoc)
		{
			unlink(c->input);
			free(c->input);
		}
		i++;
	}
	return (0);
}

