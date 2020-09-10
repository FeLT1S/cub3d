/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:29:11 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/10 19:45:20 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "libft.h"
#include "cub3d.h"

static void		splitfree(char **out)
{
	size_t		i;

	i = 0;
	while (out[i])
	{
		free(out[i]);
		i++;
	}
	free(out);
}

int		ft_colors(char **colors)
{
	int color;
	if (colors[3])
	{
		write(1, "ERROR -  MAP IS NOT VALID\n", 27);
		exit(0);
	}
	color = 0;
	color = ft_atoi(colors[0]) << 16;
	color = color | (ft_atoi(colors[1]) << 8);
	color = color | ft_atoi(colors[2]);
	return (color);
}

void	list_add(t_conf *conf, t_list *lst, char* line)
{
	char *spr_line;
	
	ft_lstadd_back(&lst, ft_lstnew(line));
	spr_line = line;
	while ((spr_line = ft_memchr(spr_line++, '2', ft_strlen(line))) && spr_line++)
		conf->numSprites++;
}

void	frame_res(char **spl_line, t_conf *conf)
{
	int width;
	int height;
	
	mlx_get_screen_size(conf->mlx, &width, &height);
	conf->width = ft_atoi(spl_line[1]);
	if (conf->width > width)
		conf->width = width;
	if (conf->width < 1)
	{
		write(1, "ERROR -  MAP IS NOT VALID\n", 27);
		exit(0);
	}
	conf->height = ft_atoi(spl_line[2]);
}

void	ft_flags(char **spl_line, char* line, t_list *lst,  t_conf *conf)
{
	if (spl_line[0] == NULL)
		return ;
	else if (!(ft_strncmp("R", spl_line[0], 2)) && !(spl_line[3]))
		frame_res(spl_line, conf);
	else if(!(ft_strncmp("NO", spl_line[0], 2)) && !(spl_line[2]))
		conf->no_path = ft_strdup(spl_line[1]);
	else if(!(ft_strncmp("SO", spl_line[0], 2)) && !(spl_line[2]))
		conf->so_path = ft_strdup(spl_line[1]);
	else if(!(ft_strncmp("WE", spl_line[0], 2)) && !(spl_line[2]))
		conf->we_path = ft_strdup(spl_line[1]);
	else if(!(ft_strncmp("EA", spl_line[0], 2)) && !(spl_line[2]))
		conf->ea_path = ft_strdup(spl_line[1]);
	else if(!(ft_strncmp("S", spl_line[0], 1)) && !(spl_line[2]))
		conf->s_path = ft_strdup(spl_line[1]);
	else if(!(ft_strncmp("F", spl_line[0], 1)) && !(spl_line[2]))
		conf->floor_col = ft_colors(ft_split(spl_line[1], ','));
	else if(!(ft_strncmp("C", spl_line[0], 1)) && !(spl_line[2]))
		conf->ceil_col = ft_colors(ft_split(spl_line[1], ','));
	else
		list_add(conf, lst, line);
}

void	check_pos(t_conf *conf, int i, int j)
{
	conf->posX = j + 0.5;
	conf->posY = i + 0.5;
	if (conf->num_spawns < 1)
		conf->num_spawns++;
	else
	{
		write(1, "ERROR -  MAP IS NOT VALID\n", 27);
		exit(0);
	}
	if (conf->map[i][j] == 'N')
	{
		conf->dirX = 0;
		conf->dirY = -1;
		conf->planeX = 1;
		conf->planeY = 0;
	}
	else if (conf->map[i][j] == 'S')
	{
		conf->dirX = 0;
		conf->dirY = 1;
		conf->planeX = -1;
		conf->planeY = 0;
	}
	else if (conf->map[i][j] == 'W')
	{
		conf->dirX = -1;
		conf->dirY = 0;
		conf->planeX = 0;
		conf->planeY = -1;
	}
	else if (conf->map[i][j] == 'E')
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

	i = 1;
	spr = 0;
	j = 0;
	while(conf->map[0][j])
	{
		if (!(ft_memchr("1 ",conf->map[0][j], 3)))
		{
			write(1, "ERROR -  MAP IS NOT VALID\n", 27);
			exit(0);
		}
		j++;
	}
	while(conf->map[i + 1])
	{
		j = 1;
		if (!(ft_memchr("1 ",conf->map[i][0], 3)))
		{
			write(1, "ERROR -  MAP IS NOT VALID\n", 27);
			exit(0);
		}
		while(conf->map[i][j + 1])
		{
			if (!(ft_memchr("NSWE 012", conf->map[i][j], 9)))
			{
				write(1, "ERROR -  MAP IS NOT VALID\n", 27);
				exit(0);
			}
			if (ft_memchr("NSWE", conf->map[i][j], 5))
				check_pos(conf, i, j);
			if (conf->map[i][j] == '0')
			{
				if (conf->map[i + 1][j] == ' ' || conf->map[i][j + 1] == ' ' || conf->map[i - 1][j] == ' ' || conf->map[i][j - 1] == ' ')
				{
					write(1, "ERROR -  MAP IS NOT VALID\n", 26);
					exit(0);
				}
			}
			if (conf->map[i][j] == '2')
			{
				conf->sprite[spr].x = j + 0.5;
				conf->sprite[spr].y = i + 0.5;
				spr++;
			}
			j++;
		}
		if (!(ft_memchr("1 ",conf->map[i][j], 3)))
		{
			write(1, "ERROR -  MAP IS NOT VALID5\n", 27);
			exit(0);
		}
		i++;
	}
	j = 0;
	while(conf->map[i][j])
	{
		if (!(ft_memchr("1 ",conf->map[i][j], 3)))
		{
			write(1, "ERROR -  MAP IS NOT VALID\n", 27);
			exit(0);
		}
		j++;
	}
	return (0);
}

void	ft_map(t_list *lst, t_conf *conf)
{
	int i;
	int num_str;
	t_list *fst_lst;
	
	fst_lst = lst;
	conf->num_spawns = 0;
	lst = lst->next;
	i = 0;
	num_str = ft_lstsize(lst);
	conf->map = (char**)malloc(sizeof(int*) * (num_str + 1));
	conf->sprite = (t_sprite*)malloc(sizeof(t_sprite) * (conf->numSprites + 1));
	while (i < num_str)
	{
		conf->map[i] = ft_strdup((char*)lst->content);
		lst = lst->next;
		i++;
	}
	conf->map[num_str + 1] = NULL;
	ft_checkmap(conf);
	ft_lstclear(&lst, free);
}

void	ft_parsing(char *file, t_conf *conf)
{
	t_list *lst;
	int fd;
	char *line;
	char **spl_line;
	
	lst = ft_lstnew(0);
	if ((fd = open(file, O_RDONLY)) < 0)
		return ;
	while ((get_next_line(fd, &line)) > 0)
	{
		spl_line = ft_split(line, ' ');
		ft_flags(spl_line, line, lst, conf);
		//free(line);
		splitfree(spl_line);
	}
	spl_line = ft_split(line, ' ');
	ft_flags(spl_line, line, lst, conf);
	//free(line);
	splitfree(spl_line);
	ft_map(lst, conf);
}