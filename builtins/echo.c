/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchauvot <gchauvot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:59:38 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/16 15:51:23 by gchauvot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	nb_args(char **args)
{
	int		size;

	size = 0;
	while (args[size])
		size++;
	return (size);
}

int		f__echo(char **args, int fdout)
{
	int		i;
	int		n_option;

	i = 1;
	n_option = 0;
	if (nb_args(args) > 1)
	{
		while (args[i] && ft_strcmp(args[i], "-n") == 0)
		{
			n_option = 1;
			i++;
		}
		while (args[i])
		{
			// QUOTE SPECIAL
			if (args[i][0] == '\"' && args[i][ft_strlen(args[i]) - 1] == '\"')
				ft_putstr_fd(ft_substr(args[i], 1, ft_strlen(args[i]) - 1), fdout);
			else
				ft_putstr_fd(args[i], fdout);
			if (args[i + 1] && args[i][0] != '\0')
				write(fdout, " ", 1);
			i++;
		}
	}	
	if (n_option == 0)
		write(1, "\n", fdout);
	return (SUCCESS);
}
