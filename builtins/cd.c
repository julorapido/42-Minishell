/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 11:37:15 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/09 17:31:01 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_error(char **args)
{
	ft_putstr_fd("cd: ", 2);
	if (args[2])
		ft_putstr_fd("string not in pwd: ", 2);
	else
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(args[1], 2);
}

static int	go_to(int n, t_minishell *t_m)
{
	char	*ev_path;

	ev_path = NULL;
	ev_path = get_env_path(t_m);
	if (n == 1)
	{
		if (!ev_path)
		{
			ft_putendl_fd("minishell : $HOME not set", STDERR);
			return (ERROR);
		}
	}
	else
	{
		ft_putstr_fd(ev_path, 1);
		ft_putstr_fd("\n", 1);
	}
	chdir(ev_path);
	return (SUCCESS);
}

static char	*gayenv(char *s, t_minishell *t_m)
{
	t_env	*tmp;
	char	**spl;
	char	*z;

	tmp = t_m->env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->value, s, FT(s)))
		{
			spl = ft_split(tmp->value, '=');
			z = ft_strdup(spl[1]);
			ft_free_split(spl);
			return (z);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

static void	update_oldpwd(t_minishell *t_m, int dd)
{
	t_env	*tmp;
	char	*s;
	char	cwd[PATH_MAX];

	tmp = t_m->env;
	s = "OLDPWD=";
	if (dd)
		s = "PWD=";
	while (tmp)
	{
		if (!ft_strncmp(tmp->value, s, FT(s)))
		{
			free(tmp->value);
			if (!dd)
				tmp->value = ft_strjoin_free(ft_strdup(s), gayenv("PWD", t_m));
			else
			{
				getcwd(cwd, PATH_MAX);
				tmp->value = ft_strjoin(s, cwd);
			}
			break ;
		}
		tmp = tmp->next;
	}
}

int	f__cd(char **args, t_minishell *t_m)
{
	int	cd_return;

	if (args[2])
	{
		ft_putendl_fd(" too many arguments", STDERR);
		return (1);
	}
	if (!args[1])
		return (go_to(1, t_m));
	if (ft_strcmp(args[1], "-") == 0 && ft_strlen(args[1]) == 1)
		return (go_to(0, t_m));
	else
	{
		update_oldpwd(t_m, 0);
		cd_return = chdir(args[1]);
		update_oldpwd(t_m, 1);
		if (cd_return < 0)
			cd_return *= -1;
		if (cd_return != 0)
			print_error(args);
	}
	return (cd_return);
}
