/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 18:37:32 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/14 18:51:36 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	check_valid(int i, int j, t_conf *conf, int *spr)
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

static void	check_border_map(int i, t_conf *conf)
{
	int		j;

	j = 0;
	while (conf->map[0][j])
	{
		if (!(ft_memchr("1 ", conf->map[i][j], 3)))
			conf->err = -1;
		j++;
	}
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
			return (conf->err = -1);
		while (conf->map[i][j + 1])
			check_valid(i, j++, conf, &spr);
		if (!(ft_memchr("1 ", conf->map[i][j], 3)))
			return (conf->err = -1);
		i++;
	}
	check_border_map(i, conf);
	if (!conf->num_spawns)
		conf->err = -1;
	return (0);
}

void		ft_map(t_list *lst, t_conf *conf, char *line, char **spl_line)
{
	int		i;
	int		num_str;
	t_list	*fst_lst;

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