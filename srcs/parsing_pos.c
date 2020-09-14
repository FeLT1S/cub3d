/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 18:40:14 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/14 18:50:31 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	check_dir_we(int i, int j, t_conf *conf)
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

static void	check_dir_ns(int i, int j, t_conf *conf)
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

void		check_pos(t_conf *conf, int i, int j)
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
