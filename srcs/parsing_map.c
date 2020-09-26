/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 18:37:32 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/25 17:24:45 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	check_valid(int i, int j, t_conf *conf, int *spr)
{
	if (!(ft_memchr("NSWE 012", conf->map[i][j], 9)))
		conf->err = 1;
	if (ft_memchr("NSWE", conf->map[i][j], 5))
		check_pos(conf, i, j);
	if (conf->map[i][j] == '0')
	{
		if (conf->map[i + 1][j] == ' ' || conf->map[i][j + 1] == ' ' ||
		conf->map[i - 1][j] == ' ' || conf->map[i][j - 1] == ' ')
			conf->err = 4;
		if (!(conf->map[i + 1][j]) || !(conf->map[i - 1][j]) ||
		!(conf->map[i][j + 1]) || !(conf->map[i][j - 1]))
			conf->err = 4;
	}
	if (conf->map[i][j] == '2')
	{
		conf->sprite[*spr].x = j + 0.5;
		conf->sprite[*spr].y = i + 0.5;
		(*spr)++;
	}
}

static void	check_border_map(int i, t_conf *conf)
{
	int		j;

	j = 0;
	while ((conf->map[i]) && conf->map[i][j])
	{
		if (!(ft_memchr("1 ", conf->map[i][j], 3)))
			conf->err = 4;
		j++;
	}
	if (conf->height == -1 || conf->width == -1)
		conf->err = 1;
	if (conf->floor_col == -1 || conf->ceil_col == -1)
		conf->err = 2;
}

static int	ft_checkmap(t_conf *conf)
{
	int		i;
	int		j;
	int		spr;

	i = 1;
	j = 1;
	spr = 0;
	check_border_map(0, conf);
	while (conf->map[i + 1])
	{
		j = 1;
		if (!(ft_memchr("1 ", conf->map[i][0], 3)))
			return (conf->err = 4);
		while (conf->map[i][j + 1])
			check_valid(i, j++, conf, &spr);
		if (!(ft_memchr("1 ", conf->map[i][j], 3)))
			return (conf->err = 4);
		i++;
	}
	check_border_map(i, conf);
	if (!conf->num_spawns)
		conf->err = 4;
	return (0);
}

void		ft_map(t_list *lst, t_conf *conf, char *line, char **spl_line)
{
	int		i;
	int		num_str;
	t_list	*fst_lst;

	if (conf->err)
		ft_error(conf, lst, line, spl_line);
	fst_lst = lst;
	conf->num_spawns = 0;
	lst = lst->next;
	i = 0;
	num_str = ft_lstsize(lst);
	if (!(conf->map = (char**)malloc(sizeof(int*) * (num_str + 1))))
		conf->err = 4;
	conf->sprite = (t_sprite*)malloc(sizeof(t_sprite) * (conf->num_sprs + 1));
	while (i < num_str)
	{
		conf->map[i] = check_strdup((char*)lst->content, conf);
		lst = lst->next;
		i++;
	}
	conf->map[num_str + 1] = NULL;
	ft_checkmap(conf);
	if (conf->err)
		ft_error(conf, fst_lst, line, spl_line);
	ft_lstclear(&fst_lst, free);
}
