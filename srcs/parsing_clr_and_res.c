/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_clr_and_res.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 18:35:58 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/14 18:49:58 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	check_colors(int color, int shift)
{
	if (color > 255 || color < 0)
		return (-1);
	else
		return (color << shift);
}

int			ft_colors(char *col_line, t_conf *conf)
{
	int		color;
	char	**colors;

	if (!(colors = ft_split(col_line, ',')))
		return (conf->err = -1);
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
	splitfree(colors);
	return (color);
}

void		frame_res(char **spl_line, t_conf *conf)
{
	int		width;
	int		height;

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
