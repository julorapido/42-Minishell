/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:32:41 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/18 13:21:41 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchauvot <gchauvot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:49:14 by gchauvot          #+#    #+#             */
/*   Updated: 2024/10/17 12:46:34 by gchauvot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc(char *eof, t_cmd *cmd, int i)
{
	char	*line;
	int		docfd;
	char	*tempfile;

	//printf("cmd: %s\n", cmd->command);
	tempfile = ft_strjoin("temp/", ft_itoa(i));
	perror("join err: ");
	printf("tempfile: %s\n", tempfile);
	docfd = open(tempfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	// docfd = open("/path/to/dir", O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
	perror("opehere err: ");
	while (1)
	{
		write(2, "heredoc>", 9);
		line = get_next_line(0);
		if (line[ft_strlen(line)-1] == '\n')
			line[ft_strlen(line)-1] = '\0';
		if (!ft_strcmp(line, eof))
			break ;
		line[ft_strlen(line)] = '\n';
		ft_putstr_fd(line, docfd);
		free(line);
	}
	line[ft_strlen(line)] = '\n';
	close(docfd);
	free(tempfile);
	free(line);
	return (docfd);
}

int	heredocalloc(t_minishell *t_m)
{
	t_cmd	*c;
	int		i;

	i = 0;
	t_m->heredocs = ft_calloc(t_m->cmd_count + 1, sizeof(int));
	while(i <= t_m->cmd_count - 1)
	{
		c = &(t_m->commands[i]);	
		if(c->is_heredoc)
			t_m->heredocs[i] = heredoc(c->input, c, i);
		i++;	
	}
	return (1);
}
int	open_files(char *filename, t_cmd *cmd, t_minishell *t_m)
{

}

int	delete_heredocs(t_minishell *t_m)
{
	int	i;
	t_cmd *c;
	char	*fname;

	i = 0;
	while(i < t_m->cmd_count)
	{
		c = &(t_m->commands[i]);
		fname = ft_strjoin("temp/", ft_itoa(i));
		if(c->is_heredoc)
		{
			unlink(fname);
			perror("unlink1");
		}
		free(fname);
		i++;
	}
}
