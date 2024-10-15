/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchauvot <gchauvot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 12:09:55 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/14 15:47:43 by gchauvot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "pipex.h"
#include "minishell.h"

// #1 Initiate Pipe						||	Pipe openned for parent && 
// #2 Create fork (child process)		||	fork wait for child process 
// #3 Child run [execve] and to pipe	||	
// #4 Pending parent take back control	||	parent is back [<=<=]
// ---------------------------     --------------------------------
// #++ Since we let child process write, we close its Read-End (pfd[0]). 
// #++ And since parent process reads, we close the Write-End (pfd[1]).
// ---------------------------     ------------------------------------
// #5 Use STDOUT && STDIN for pipe 		||  instead of random file ds

// void	ft_execve(char *f_name, char *cmd, char **env, int is_parent)
// {
// 	char	**ft_cmd;
// 	char	*path;
// 	int		fd;

// 	fd = open_file(f_name, is_parent);
// 	dup2(fd, is_parent);
// 	close(fd);
// 	ft_cmd = ft_split(cmd, ' ');
// 	path = bget_path2(ft_cmd[0], env);
// 	if (execve(path, ft_cmd, env) == -1)
// 	{
// 		ft_putstr_fd("pipex: command not found: ", 2);
// 		ft_putendl_fd(ft_cmd[0], 2);
// 		ft_free_tab(ft_cmd);
// 		exit(0);
// 	}
// }

void	ft_execve(char *cmd, char **env, pid_t *pid, t_pipes piper)
{
	char	**ft_cmd;
	char	*path;
	int		built;

	ft_cmd = ft_split(cmd, ' ');
	built = is_builtin(ft_cmd[0]);
	path = bget_path2(ft_cmd[0], env);
	if(built != -1)
	{
		run_builtin(piper.t_m, built);
		exit(0);
	}
	if (execve(path, ft_cmd, env) == -1)
	{
		ft_putstr_fd("pipex: command not found: ", STDERR_FILENO);
		ft_putendl_fd(ft_cmd[0], 2);
		ft_free_tab(ft_cmd);
		free(pid);
		exit(0);
	}
}
int	hereorfile(t_pipes pipex)
{
	if (!ft_strcmp(pipex.argv[1], "here_doc"))
	{
		return (open_file(".heredoc_tmp", STDIN_FILENO));
	}
	else
		return (open_file(pipex.argv[1], STDIN_FILENO));
}

int	childprocess1(t_pipes pipex, char *cmd, int i)
{
	int		pfd[2];
	int		fdin;

	if (pipe(pfd) == -1)
		exit(-1);
	pipex.pid[i-2] = fork();
	if (pipex.pid[i-2] == -1)
		exit(-1);
	if (!pipex.pid[i-2])
	{
		close(pipex.save);
		if (i == 2 || (!ft_strcmp(pipex.argv[1], "here_doc") && i == 3))
		{
			fdin = hereorfile(pipex);
			dup2(fdin, STDIN_FILENO);
			close(fdin);
		}
			dup2(pfd[STDOUT_FILENO], STDOUT_FILENO);
			close(pfd[STDIN_FILENO]);
			close(pfd[STDOUT_FILENO]);
			ft_execve(cmd, pipex.env, pipex.pid, pipex);
	}
	//fprintf(stderr, "ftchild, partenside\n");
	dup2(pfd[STDIN_FILENO], STDIN_FILENO);
	close(pfd[STDIN_FILENO]);
	close(pfd[STDOUT_FILENO]);
	return 1;
}

int	lastprocess(char *cmd,t_pipes pipex, int i)
{
	// fprintf(stderr, "LAST  i: %d, cmd: %s\n", i, cmd);
	pipex.pid[i-2] = fork();
	if (pipex.pid[i-2] == -1)
		exit(-1);
	if (!pipex.pid[i-2])
	{
		close(pipex.save);
		int fdout = open_file(pipex.argv[pipex.argc-1], STDOUT_FILENO);
		dup2(fdout, STDOUT_FILENO);
		close(fdout);
		ft_execve(cmd, pipex.env, pipex.pid, pipex);
	}
	return 1;
}

t_pipes	initstruc(t_pipes *pipex, int argc, char **argv, char **env)
{
	pipex->argc = argc;
	pipex->argv = argv;
	pipex->env = env;
	pipex->save = dup(0);
	if (!ft_strcmp(argv[1], "here_doc"))
		pipex->xcmd = argc - 4;
	else
		pipex->xcmd = argc - 3;
	pipex->pid = calloc((pipex->xcmd), sizeof(pid_t));
	if (!pipex->pid)
	{
		perror("pipex: ");
		exit(0);
	}
	return(*pipex);
}

int	pipex(int argc, char **argv, char **env, t_minishell *t_m)
{
	int		i;
	t_pipes	pipex;
	int		x;

	i = 1;
	x = 0;
	pipex = initstruc(&pipex, argc, argv, env);
	pipex.t_m = t_m;
	if (!ft_strcmp(argv[1], "here_doc"))
	{	
		heredoc(argv);
		x = 1;
	}
	while (i++ <= pipex.xcmd - 1)
		childprocess1(pipex, argv[x + i], i);	
	lastprocess(argv[x + i], pipex, i);
	i = 0;
	dup2(pipex.save, 0);
	close(pipex.save);
	while(i <= (pipex.xcmd-1))
		waitpid(pipex.pid[i++], 0, 0);
	free(pipex.pid);
	if (!access(".heredoc_tmp", F_OK))
		unlink(".heredoc_tmp");
	return (1);
}


// int	main(int argc, char **argv, char **env)
// {
// 	if (argc <= 4)
// 		exit_handler(-1);	
// 	pipex(argc, argv, env);
// 	// int fd=4;
// 	// fd = open("oui", O_RDWR);
// 	// // dup2(fd,0);
// 	// // close(fd);
// 	//  write(0, "abad", 4);
// 	//  char buf[100];
// 	//  read(0,buf,10);
// 	// printf("yolo");

	
// 	return(0);
// }

char *heredoc(char **argv)
{
	// char	*line;
	// int		docfd;
	// docfd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	// while (1)
	// {
	// 	write(2, "heredoc>", 9);
	// 	line = get_next_line(0);
	// 	if (line[ft_strlen(line)-1] == '\n')
	// 		line[ft_strlen(line)-1] = '\0';
	// 	if (!ft_strcmp(line, argv[2]))
	// 		break ;
	// 	line[ft_strlen(line)] = '\n';
	// 	ft_putstr_fd(line, docfd);
	// 	free(line);
	// }
	// line[ft_strlen(line)] = '\n';
	// close(docfd);
	// free(line);
	return (".heredoc_tmp");
}

