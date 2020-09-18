/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 19:34:07 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/18 17:07:48 by jiandre          ###   ########.fr       */
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