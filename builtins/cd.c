/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 11:37:15 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/14 14:43:54 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		print_error(char **args)
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
	if (n == 1)
	{
		ev_path = get_env_path(t_m);
		if(!ev_path)
		{
			ft_putendl_fd("minishell : $HOME not set", STDERR);
			return (ERROR);
		}
	}else
	{
			
	}
	chdir(ev_path);
	ft_memdel(ev_path);	
	return (SUCCESS);
}

static void update_oldpwd(t_minishell *t_m)
{
	
}

int	f__cd(char **args, t_minishell *t_m)
{
	int	cd_return;

	if (!args[1])
		return(go_to(1, t_m));	
	if (ft_strcmp(args[1], "-") == 0)
		return(go_to(0, t_m));
	else
	{
		update_oldpwd(t_m);
		cd_return = chdir(args[1]);
		if (cd_return < 0)
			cd_return *= -1;
		if (cd_return != 0)
			print_error(args);
	}
	return (cd_return);
}
