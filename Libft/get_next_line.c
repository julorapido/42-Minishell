/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julessainthorant <marvin@42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 10:14:25 by julessainthor     #+#    #+#             */
/*   Updated: 2024/07/15 17:58:53 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_free(char *buffer, char *buf)
{
	char	*temp;

	if (buf == NULL)
		return (NULL);
	temp = ft_strjoin(buffer, buf);
	free(buffer);
	return (temp);
}

static char	*ft_next_line(char *s_bfr)
{
	char	*next_line;
	int		i;
	int		j;

	i = 0;
	while (s_bfr[i] && s_bfr[i] != '\n')
		i++;
	if (!s_bfr[i])
	{
		free(s_bfr);
		return (NULL);
	}
	next_line = (char *)ft_calloc((ft_strlen(s_bfr) - i) + 1, (sizeof(char)));
	i++;
	j = 0;
	while (s_bfr[i])
		next_line[j++] = s_bfr[i++];
	free(s_bfr);
	return (next_line);
}

static char	*ft_line(char *buffer)
{
	char	*s;
	int		i;

	i = 0;
	if (!buffer[i])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	s = (char *) ft_calloc((i + 2), sizeof(char));
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		s[i] = buffer[i];
		i++;
	}
	if (buffer[i] && buffer[i] == '\n')
		s[i++] = '\n';
	return (s);
}

static char	*read_file(int fd_, char *s)
{
	char	*b;
	int		bytes_r;

	if (!s)
		s = (char *) ft_calloc(1, sizeof(char));
	b = (char *)(ft_calloc(BUFFER_SIZE + 1, sizeof(char)));
	bytes_r = 1;
	while (bytes_r > 0)
	{
		bytes_r = read(fd_, b, BUFFER_SIZE);
		if (bytes_r == -1)
		{
			free(b);
			free(s);
			return (NULL);
		}
		b[bytes_r] = '\0';
		s = ft_free(s, b);
		if (ft_strchr(b, '\n'))
			break ;
	}
	free(b);
	return (s);
}

char	*get_next_line(int fd)
{
	static char	*bfr;
	char		*line;

	if (read(fd, 0, 0) < 0)
	{
		free(bfr);
		bfr = NULL;
		return (NULL);
	}
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	bfr = read_file(fd, bfr);
	if (!bfr)
		return (NULL);
	line = ft_line(bfr);
	bfr = ft_next_line(bfr);
	return (line);
}

/*
int main(int argc, char **argv)
{
	char* fileName = "file.txt";
    int fd = open(fileName, O_RDWR);

    if(fd == -1){
		printf("\nError Opening File!!\n");
		exit(1);
    }
    else{
		printf("\nFile %s opened sucessfully!\n", fileName);
    }
	fd = 0;
	printf("%s \n", get_next_line(fd));
	printf("%s \n", get_next_line(fd));
	printf("%s \n", get_next_line(fd));
	printf("%s \n", get_next_line(fd));
	printf("%s \n", get_next_line(fd));
	printf("%s \n", get_next_line(fd));
	return 0;
}*/
