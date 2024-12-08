/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_multisplit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julessainthorant <marvin@42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 10:47:40 by julessainthor     #+#    #+#             */
/*   Updated: 2024/12/08 10:48:00 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_multisplit(t_mltsplit *s)
{
	int	i;

	i = 0;
	while (i < (*s).mltsplit_l)
	{
		if (s[i].s)
			free(s[i].s);
		i++;
	}
	free(s);
}
