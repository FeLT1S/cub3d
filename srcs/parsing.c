/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:29:11 by jiandre           #+#    #+#             */
/*   Updated: 2020/08/27 20:01:49 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "libft/inc/libft.h"

char	**ft_parsing(char *file)
{
	int fd;
	int i;
	char str[128];
	char *s;
	int str_count;
	char **map;
	
	fd = open(file, O_RDONLY);
	s = &str[0];
	while((read(fd, str, 128)))
	{
		while ((s = ft_memchr(s, '\n', 128)))
		{
			str_count++;
			s++;
		}
	}
	close(fd);
	fd = open(file, O_RDONLY);
	map = (char**)malloc(sizeof(char*) * (str_count + 1));
	map[str_count + 1] = 0;
	i = 0;
	while((get_next_line(fd, &map[i])) > 0)
		i++;
	return(map);
}
