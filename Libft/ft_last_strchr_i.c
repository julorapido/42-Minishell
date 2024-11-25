
#include "libft.h"

int	ft_last_strchr_i(const char *string, char a, char b)
{
	char	*str;
	int		i;
    int     ix;

	i = 0;
    ix = -1;
	str = (char *)string;
	while (*str != 0)
	{
        if (*str == a || *str == b)
            ix = i;
		str++;
		i++;
	}
	return (ix);
}

