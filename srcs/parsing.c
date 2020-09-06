/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:29:11 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/06 18:35:17 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "libft.h"
#include "cub3d.h"

int		ft_strapi(char *str, const char **arr_str, size_t len)
{
	int i;

	i = 0;
	while (arr_str)
	{
		if(!ft_strncmp(str, arr_str[i], len))
			return(1);
		i++;
	}
	return (0);
}

int		ft_colors(char **colors)
{
	int color;
	
	color = 0;
	color = ft_atoi(colors[0]) << 16;
	color = color | (ft_atoi(colors[1]) << 8);
	color = color | ft_atoi(colors[2]);
	return (color);
}

void	ft_flags(char **spl_line, t_conf *conf)
{
	if (!(ft_strncmp("R", spl_line[0], 2)))
	{
		conf->width = ft_atoi(spl_line[1]);
		conf->height = ft_atoi(spl_line[2]);
	}
	else if(!(ft_strncmp("NO", spl_line[0], 2)))
		conf->no_path = spl_line[1];
	else if(!(ft_strncmp("SO", spl_line[0], 2)))
		conf->so_path = spl_line[1];
	else if(!(ft_strncmp("WE", spl_line[0], 2)))
		conf->we_path = spl_line[1];
	else if(!(ft_strncmp("EA", spl_line[0], 2)))
		conf->ea_path = spl_line[1];
	else if(!(ft_strncmp("S", spl_line[0], 1)))
		conf->s_path = spl_line[1];
	else if(!(ft_strncmp("F", spl_line[0], 1)))
		conf->floor_col = ft_colors(ft_split(spl_line[1], ','));
	else if(!(ft_strncmp("C", spl_line[0], 1)))
		conf->ceil_col = ft_colors(ft_split(spl_line[1], ','));
}

void	check_pos(t_conf *conf, int i, int j)
{
	conf->posX = j + 0.5;
	conf->posY = i + 0.5;
	if (conf->map[i][j] == 'N')
	{
		conf->dirX = 0;
		conf->dirY = -1;
		conf->planeX = 1;
		conf->planeY = 0;
	}
	if (conf->map[i][j] == 'S')
	{
		conf->dirX = 0;
		conf->dirY = 1;
		conf->planeX = -1;
		conf->planeY = 0;
	}
	if (conf->map[i][j] == 'W')
	{
		conf->dirX = -1;
		conf->dirY = 0;
		conf->planeX = 0;
		conf->planeY = -1;
	}
	if (conf->map[i][j] == 'E')
	{
		conf->dirX = 1;
		conf->dirY = 0;
		conf->planeX = 0;
		conf->planeY = 1;
	}
	conf->map[i][j] = '0';
}

int	ft_checkmap(t_conf *conf)
{
	int j;
	int i;
	int spr;

	i = 0;
	spr = 0;
	while(conf->map[i])
	{
		j = 0;
		while(conf->map[i][j])
		{
			if (conf->map[i][j] != '0' && conf->map[i][j] != '1' && conf->map[i][j] != '2' && conf->map[i][j] != 'N' && conf->map[i][j] != 'S' && conf->map[i][j] != 'W' && conf->map[i][j] != 'E' && conf->map[i][j] != ' ')
			{
				write(1, "ERROR -  MAP IS NOT VALID", 26);
				return (1);
			}
			if (conf->map[i][j] == 'N' || conf->map[i][j] == 'S' || conf->map[i][j] == 'W' || conf->map[i][j] == 'E')
				check_pos(conf, i, j);
			j++;
			if (conf->map[i][j] == '2')
			{
				conf->sprite[spr].x = j + 0.5;
				conf->sprite[spr].y = i + 0.5;
				spr++;
			}
		}
		i++;
	}
	return (0);
}

void	ft_map(t_list *lst, t_conf *conf)
{
	int i;
	int num_str;
	t_list *fst_lst;
	
	fst_lst = lst;
	i = 0;
	num_str = ft_lstsize(lst);
	conf->map = (char**)malloc(sizeof(int*) * (num_str + 1));
	conf->sprite = (t_sprite*)malloc(sizeof(t_sprite) * (conf->numSprites + 1));
	while (i < num_str)
	{
		conf->map[i] = (char*)lst->content;
		lst = lst->next;
		i++;
	}
	conf->map[num_str + 1] = NULL;
	ft_checkmap(conf);
}

void	ft_parsing(char *file, t_conf *conf)
{
	t_list *lst;
	int fd;
	char *line;
	char **spl_line;
	char *spr_line;
	
	if ((fd = open(file, O_RDONLY)) < 0)
		return ;
	while ((get_next_line(fd, &line)) > 0)
	{
		spl_line = ft_split(line, ' ');
		ft_flags(spl_line, conf);
		if(spl_line[0][0] == '1' || spl_line[0][0] == '0')
		{
			ft_lstadd_back(&lst, ft_lstnew(line));
			spr_line = line;
			while ((spr_line = ft_memchr(spr_line++, '2', ft_strlen(line))) && spr_line++)
				conf->numSprites++;
		}
	}
	spl_line = ft_split(line, ' ');
	ft_flags(spl_line, conf);
	if(spl_line[0][0] == '1' || spl_line[0][0] == '0')
	{
		ft_lstadd_back(&lst, ft_lstnew(line));
		spr_line = line;
		while ((spr_line = ft_memchr(spr_line++, '2', ft_strlen(line))) && spr_line++)
			conf->numSprites++;
	}
	ft_map(lst, conf);
}