/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:29:11 by jiandre           #+#    #+#             */
/*   Updated: 2020/08/31 17:07:57 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "libft.h"
#include "cub3d.h"

int		ft_strapi(char *str, char **arr_str, size_t len)
{
	while (*arr_str)
	{
		if(!ft_strncmp(str, *arr_str, len))
			return(1);
		*arr_str++;
	}
	return (0);
}

int		ft_colors(char **colors)
{
	int color;

	color = ft_atoi(colors[1]) << 8;
	color = ft_atoi(colors[2]) << 4;
	color = ft_atoi(colors[3]);
	return (color);
}

char	**ft_flags(char **spl_line, t_conf conf)
{
	char **colors;
	
	if (!(ft_strncmp("R ", spl_line[0], 2)))
	{
		conf.width = spl_line[1];
		conf.height = spl_line[2];
	}
	else if(!(ft_strncmp("NO ", spl_line[0], 3)))
		conf.no_path = spl_line[1];
	else if(!(ft_strncmp("SO ", spl_line[0], 3)))
		conf.no_path = spl_line[1];
	else if(!(ft_strncmp("WE ", spl_line[0], 3)))
		conf.no_path = spl_line[1];
	else if(!(ft_strncmp("EA ", spl_line[0], 3)))
		conf.no_path = spl_line[1];
	else if(!(ft_strncmp("S ", spl_line[0], 2)))
		conf.no_path = spl_line[1];
	else if(!(ft_strncmp("F ", spl_line[0], 2)))
		conf.floor_col = ft_colors(ft_split(spl_line[1], ','));
	else if(!(ft_strncmp("C ", spl_line[0], 2)))
		conf.ceil_col = ft_colors(ft_split(spl_line[1], ','));
}

char	**ft_parsing(char *file, t_conf conf)
{
	const char *flgs[9] = {"R ", "NO ", "SO ", "WE ", "EA ", "S ", "F ", "C ", NULL};
	const char *map[4] = {"0", "1", NULL};
	int fd;
	char *line;
	char **spl_line;

	if(!(fd = open(file, O_RDONLY)));
		return (NULL);
	while ((get_next_line(fd, &line)) > 0)
	{
		spl_line = ft_split(line, ' ');
		if (ft_strapi(spl_line[0], flgs, 3))
			ft_flags(spl_line, conf);
		if (ft_strapi(spl_line[0], map, 2))
			...
	}
}