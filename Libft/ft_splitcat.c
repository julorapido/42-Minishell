/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitcat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:35:01 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/16 17:45:32 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_splitcat(char **ft_split)
{
	int		i;
	int		j;
	int		k;
	size_t	s;
	char	*ret;

	i = 0;
	s = 0;
	while(ft_split[i])
	{
		s += ft_strlen(ft_split[i]);
		i++;
	}
	ret = (char *) malloc(s * sizeof(char));
	if (!ret)
		return (NULL);
	k = 0;
	i = 0;
	while(ft_split[i])
	{
		j = 0;
		while(ft_split[i][j] != '\0')
		{
			ret[k] = ft_split[i][j];
			k++;
			j++;
		}
		//free(ft_split[i]);
		i++;
	}
	return (ret);
}
