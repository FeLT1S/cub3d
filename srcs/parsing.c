/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:29:11 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/01 19:51:00 by jiandre          ###   ########.fr       */
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

void	ft_map(t_list *lst, t_conf *conf)
{
	int i;
	int num_str;
	t_list *fst_lst;
	
	fst_lst = lst;
	i = 0;
	num_str = ft_lstsize(lst);
	conf->map = (char**)malloc(sizeof(int*) * num_str);
	while (i < num_str)
	{
		conf->map[i] = (char*)lst->content;
		lst = lst->next;
		i++;
	}
	//ft_lstclear(&fst_lst, );
}

void	ft_parsing(char *file, t_conf *conf)
{
	t_list *lst;
	int fd;
	char *line;
	char **spl_line;
	
	if ((fd = open(file, O_RDONLY)) < 0)
		return ;
	while ((get_next_line(fd, &line)) > 0)
	{
		spl_line = ft_split(line, ' ');
		ft_flags(spl_line, conf);
		if(spl_line[0][0] == '1' || spl_line[0][0] == '0')
			ft_lstadd_back(&lst, ft_lstnew(line));
	}
	spl_line = ft_split(line, ' ');
	ft_flags(spl_line, conf);
	if(spl_line[0][0] == '1' || spl_line[0][0] == '0')
		ft_lstadd_back(&lst, ft_lstnew(line));
	ft_map(lst, conf);
}