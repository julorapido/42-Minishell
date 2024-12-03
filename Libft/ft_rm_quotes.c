/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rm_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julessainthorant <marvin@42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 14:52:40 by julessainthor     #+#    #+#             */
/*   Updated: 2024/12/03 14:52:58 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_rm_quotes(char *s)
{
	size_t	i;
	bool	dq;
	bool	sq;
	int		j;

	i = 0;
	j = 0;
	sq = 0;
	dq = 0;
	while (s[i])
	{
		if (s[i] == '"' && !sq)
			dq = !dq;
		else if (s[i] == '\'' && !dq)
			sq = !sq;
		else
			s[j++] = s[i];
		i++;
	}
	s[j] = '\0';
	return (s);
}
