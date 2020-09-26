/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 18:40:14 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/25 17:05:39 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	check_dir_we(int i, int j, t_conf *conf)
{
	if (conf->map[i][j] == 'W')
	{
		conf->dirx = -1;
		conf->diry = 0;
		conf->plx = 0;
		conf->ply = -1;
	}
	else if (conf->map[i][j] == 'E')
	{
		conf->dirx = 1;
		conf->diry = 0;
		conf->plx = 0;
		conf->ply = 1;
	}
}

static void	check_dir_ns(int i, int j, t_conf *conf)
{
	if (conf->map[i][j] == 'N')
	{
		conf->dirx = 0;
		conf->diry = -1;
		conf->plx = 1;
		conf->ply = 0;
	}
	else if (conf->map[i][j] == 'S')
	{
		conf->dirx = 0;
		conf->diry = 1;
		conf->plx = -1;
		conf->ply = 0;
	}
}

void		check_pos(t_conf *conf, int i, int j)
{
	conf->posx = j + 0.5;
	conf->posy = i + 0.5;
	if (conf->num_spawns < 1)
		conf->num_spawns++;
	else
		conf->err = 4;
	if (conf->map[i][j] == 'N' || conf->map[i][j] == 'S')
		check_dir_ns(i, j, conf);
	if (conf->map[i][j] == 'W' || conf->map[i][j] == 'E')
		check_dir_we(i, j, conf);
	conf->map[i][j] = '0';
}
