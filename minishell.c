/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:07:22 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/09 16:05:12 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>
int	sigcatch = 0;

static void	shlvlhandler(char **env)
{
	int		i;
	size_t	j;
	int		mark;
	char	*res;

	i = 0;
	j = 0;
	mark = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "SHLVL=", 6))
		{
			res = ft_itoa(ft_atoi(&env[i][6]) + 1);
			while (j < ft_strlen(res))
			{
				env[i][6 + j] = res[j];
				j++;
			}
			free(res);
			env[i][6 + j] = '\0';
			mark = -1;
		}
		i++;
	}
	if (mark != -1)
	{
		env[i] = "SHLVL=0";
		env[i + 1] = 0;
	}
}

static char	**envnull(char	*argname)
{
	char	**env;
	char	cwd[PATH_MAX];

	env = ft_calloc(4, sizeof(char *));
	if (!env)
		return (NULL);
	if (!getcwd(cwd, PATH_MAX))
		cwd[0] = NULL;
	env[0] = ft_strjoin("PWD=", cwd);
	env[1] = ft_strdup("SHLVL=0");
	env[2] = ft_strjoin("_=", argname);
	env[3] = NULL;
	return (env);
}

static void	minishell(t_minishell *t_m)
{
	char	*line;

	signalignore(SIGQUIT);
	signalsetter(SIGINT, handler);
	t_m->exstat = 0;
	t_m->set = ft_strdup("><");
	line = readline("$ ");
	add_history(line);
	while (line)
	{
		if (FT(line) < 1 || !ft_strcmp(line, " ") || P_ER(line) != '\0')
		{
			if (P_ER(line) != '\0')
				printf("zsh: parse error near `%c'\n", P_ER(line));
			line = readline("$ ");
			add_history(line);
			continue ;
		}
		fdp_parsing(line, t_m);
		//exec_cmds(t_m);
		free_commands(t_m);
		line = readline("$ ");
		add_history(line);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*t_m;	

	(void)argv;
	if (!env || argc > 1)
		return (0);
	if (!*env)
		env = envnull(argv[0]);
	shlvlhandler(env);
	t_m = ft_calloc(1, sizeof(t_minishell));
	if (!t_m)
		return (EXIT_FAILURE);
	env_init(t_m, env);
	minishell(t_m);
	exit(EXIT_SUCCESS);
}
