/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_INC.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:15:06 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/06 12:17:49 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char    ft_INC(char q, char si)
{
    if (!q)
    {
        return (si);
    }
    else
    {
        if(si == q)
            return ('\0');
        else
            return (q);
    }
}