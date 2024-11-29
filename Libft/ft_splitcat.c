/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitcat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:35:01 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/28 12:37:19 by jsaintho         ###   ########.fr       */
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
	ret = (char *) malloc((s + 1) * sizeof(char));
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
		free(ft_split[i]);
		i++;
	}
	ret[k] = '\0';
	return (ret);
}
