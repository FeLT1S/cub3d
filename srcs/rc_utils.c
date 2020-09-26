/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 19:34:07 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/26 16:52:51 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		sort(int *sprite_order, double *spritedist, int amount)
{
	double	buff;
	int		i;
	int		check;

	check = 1;
	while (check)
	{
		i = 0;
		check = 0;
		while (i < amount - 1)
		{
			if (spritedist[i] < spritedist[i + 1])
			{
				check = 1;
				buff = spritedist[i];
				spritedist[i] = spritedist[i + 1];
				spritedist[i + 1] = buff;
				buff = sprite_order[i];
				sprite_order[i] = sprite_order[i + 1];
				sprite_order[i + 1] = buff;
			}
			i++;
		}
	}
}

void		my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->l_ln + x * (data->bpp / 8));
	*(unsigned int*)dst = color;
}

void		code_error(t_conf *conf)
{
	if (conf->err == 1)
		write(2, "Not valid resolution\n", 22);
	else if (conf->err == 2)
		write(2, "Not valid color\n", 17);
	else if (conf->err == 3)
		write(2, "Not valid path\n", 16);
	else if (conf->err == 4)
		write(2, "Not valid map\n", 15);
	else if (conf->err == 5)
		write(2, "Not valid texture\n", 19);
	else if (conf->err == 6)
		write(2, "Not valid extension\n", 21);
	else if (conf->err == 7)
		write(2, "Cannot open file\n", 17);
	else if (conf->err == 8)
		write(2, "Program flag error\n", 20);
	else if (conf->err == 9)
		write(2, "File has bad symbols\n", 22);
	else
		write(2, "Other error\n", 13);
}

void		nval(t_conf *conf, int err)
{
	conf->err = err;
	ft_error(conf, 0, 0, 0);
}

int			ft_close(t_conf *conf)
{
	if (conf->data.img)
		mlx_destroy_image(conf->mlx, conf->data.img);
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
	exit(0);
}
