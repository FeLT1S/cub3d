/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:29:11 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/12 20:13:00 by jiandre          ###   ########.fr       */
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
		if (out[i])
			free(out[i]);
		i++;
	}
	free(out);
}

void	ft_error(t_conf *conf, t_list *lst, char *line, char **spl_line)
{
	write(1, "ERROR -  MAP IS NOT VALID\n", 27);
	if (conf->no_path)
		free(conf->no_path);
	if (conf->so_path)
		free(conf->so_path);
	if (conf->we_path)
		free(conf->we_path);
	if (conf->ea_path)
		free(conf->ea_path);
	if (conf->s_path)
		free(conf->s_path);
	if (conf->map)
		splitfree(conf->map);
	if (conf->sprite)
		free(conf->sprite);
	if (lst)
		ft_lstclear(&lst, free);
	if (line)
		free(line);
	if (spl_line)
		splitfree(spl_line);
	exit(0);
}

char	*check_strdup(const char *s, t_conf *conf)
{
	char *new_str;
	
	if (!(new_str = ft_strdup(s)))
		conf->err = -1;
	return (new_str);
}

int		check_colors(int color, int shift)
{
	if (color > 255 || color < 0)
		return (-1);
	else
		return(color  << shift);
}

int		ft_colors(char **colors, t_conf *conf)
{
	int color;
	
	if (colors[3])
		conf->err = -1;
	color = 0;
	color = check_colors(ft_atoi(colors[0]), 16) != -1;
	if (color == -1)
		conf->err = -1;
	color = color | check_colors(ft_atoi(colors[1]), 8);
	if (color == -1)
		conf->err = -1;
	color = color | check_colors(ft_atoi(colors[2]), 0);
	if (color == -1)
		conf->err = -1;
	return (color);
}

void	list_add(t_conf *conf, t_list *lst, char *line)
{
	char *spr_line;
	char *new_line;

	new_line = check_strdup(line, conf);
	ft_lstadd_back(&lst, ft_lstnew(new_line));
	spr_line = new_line;
	while ((spr_line = ft_memchr(spr_line++, '2', ft_strlen(line))) && spr_line++)
		conf->num_sprs++;
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
		conf->err = -1;
	conf->height = ft_atoi(spl_line[2]);
		if (conf->height > height)
		conf->height = height;
	if (conf->height < 1)
		conf->err = -1;
}

void	ft_flags(char **spl_line, char* line, t_list *lst,  t_conf *conf)
{
	if (spl_line[0] == NULL)
		return ;
	else if (!(ft_strncmp("R", spl_line[0], 2)) && !(spl_line[3]))
		frame_res(spl_line, conf);
	else if(!(ft_strncmp("NO", spl_line[0], 2)) && !(spl_line[2]))
		conf->no_path = check_strdup(spl_line[1], conf);
	else if(!(ft_strncmp("SO", spl_line[0], 2)) && !(spl_line[2]))
		conf->so_path = check_strdup(spl_line[1], conf);
	else if(!(ft_strncmp("WE", spl_line[0], 2)) && !(spl_line[2]))
		conf->we_path = check_strdup(spl_line[1], conf);
	else if(!(ft_strncmp("EA", spl_line[0], 2)) && !(spl_line[2]))
		conf->ea_path = check_strdup(spl_line[1], conf);
	else if(!(ft_strncmp("S", spl_line[0], 1)) && !(spl_line[2]))
		conf->s_path = check_strdup(spl_line[1], conf);
	else if(!(ft_strncmp("F", spl_line[0], 1)) && !(spl_line[2]))
		conf->floor_col = ft_colors(ft_split(spl_line[1], ','), conf);
	else if(!(ft_strncmp("C", spl_line[0], 1)) && !(spl_line[2]))
		conf->ceil_col = ft_colors(ft_split(spl_line[1], ','), conf);
	else
		list_add(conf, lst, line);
}

void	check_dir_we(int i, int j, t_conf *conf)
{
	if (conf->map[i][j] == 'W')
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
}

void	check_dir_ns(int i, int j, t_conf *conf)
{
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
}

void	check_pos(t_conf *conf, int i, int j)
{
	conf->posX = j + 0.5;
	conf->posY = i + 0.5;
	if (conf->num_spawns < 1)
		conf->num_spawns++;
	else
		conf->err = -1;
	if (conf->map[i][j] == 'N' || conf->map[i][j] == 'S')
		check_dir_ns(i, j, conf);
	if (conf->map[i][j] == 'W' || conf->map[i][j] == 'E')
		check_dir_we(i, j, conf);
	conf->map[i][j] = '0';
}

void		check_valid(int i, int j, t_conf *conf, int *spr)
{
	if (!(ft_memchr("NSWE 012", conf->map[i][j], 9)))
		conf->err = -1;
	if (ft_memchr("NSWE", conf->map[i][j], 5))
		check_pos(conf, i, j);
	if (conf->map[i][j] == '0')
	{
		if (conf->map[i + 1][j] == ' ' || conf->map[i][j + 1] == ' ' || 
		conf->map[i - 1][j] == ' ' || conf->map[i][j - 1] == ' ')
			conf->err = -1;
	}
	if (conf->map[i][j] == '2')
	{
		conf->sprite[*spr].x = j + 0.5;
		conf->sprite[*spr].y = i + 0.5;
		(*spr)++;
	}
}

void	check_border_map(int i, t_conf *conf)
{
	int j;

	j = 0;
	while(conf->map[0][j])
	{
		if (!(ft_memchr("1 ",conf->map[i][j], 3)))
			conf->err = -1;
		j++;
	}
}

int	ft_checkmap(t_conf *conf)
{
	int i;
	int j;
	int spr;

	i = 1;
	j = 1;
	spr = 0;
	check_border_map(0, conf);
	while(conf->map[i + 1])
	{
		j = 1;
		if (!(ft_memchr("1 ",conf->map[i][0], 3)))
			return(conf->err = -1);
		while(conf->map[i][j + 1])
		{
			check_valid(i, j, conf, &spr);
			j++;
		}
		if (!(ft_memchr("1 ",conf->map[i][j], 3)))
			return(conf->err = -1);
		i++;
	}
	check_border_map(i, conf);
	return (0);
}

void	ft_map(t_list *lst, t_conf *conf, char *line, char **spl_line)
{
	int i;
	int num_str;
	t_list *fst_lst;
	
	if (conf->err == -1)
		ft_error(conf, lst, line, spl_line);
	fst_lst = lst;
	conf->num_spawns = 0;
	lst = lst->next;
	i = 0;
	num_str = ft_lstsize(lst);
	if (!(conf->map = (char**)malloc(sizeof(int*) * (num_str + 1))))
		conf->err = -1;
	conf->sprite = (t_sprite*)malloc(sizeof(t_sprite) * (conf->num_sprs + 1));
	while (i < num_str)
	{
		conf->map[i] = check_strdup((char*)lst->content, conf);
		lst = lst->next;
		i++;
	}
	conf->map[num_str + 1] = NULL;
	ft_checkmap(conf);
	if (conf->err == -1)
		ft_error(conf, fst_lst, line, spl_line);
	ft_lstclear(&fst_lst, free);
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
		if (!(spl_line = ft_split(line, ' ')))
			ft_error(conf, lst, line, spl_line);
		ft_flags(spl_line, line, lst, conf);
		free(line);
		line = 0;
		splitfree(spl_line);
		spl_line  = 0;
	}
	if (!(spl_line = ft_split(line, ' ')))
		ft_error(conf, lst, line, spl_line);
	ft_flags(spl_line, line, lst, conf);
	free(line);
	line = 0;
	splitfree(spl_line);
	spl_line  = 0;
	ft_map(lst, conf, line, spl_line);
}
