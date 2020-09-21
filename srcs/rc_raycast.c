/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc_raycast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 19:31:13 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/21 13:35:46 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		rc_init(t_rc *rc, t_conf *conf)
{
	rc->mapx = (int)conf->posx;
	rc->mapy = (int)conf->posy;
	rc->ddistx = fabs(1 / rc->raydirx);
	rc->ddisty = fabs(1 / rc->raydiry);
	rc->hit = 0;
	if (rc->raydirx < 0)
	{
		rc->stepx = -1;
		rc->sidedistx = (conf->posx - rc->mapx) * rc->ddistx;
	}
	else
	{
		rc->stepx = 1;
		rc->sidedistx = (rc->mapx + 1.0 - conf->posx) * rc->ddistx;
	}
	if (rc->raydiry < 0)
	{
		rc->stepy = -1;
		rc->sidedisty = (conf->posy - rc->mapy) * rc->ddisty;
	}
	else
	{
		rc->stepy = 1;
		rc->sidedisty = (rc->mapy + 1.0 - conf->posy) * rc->ddisty;
	}
}

void		rc_dda(t_rc *rc, t_conf *conf)
{
	while (rc->hit == 0)
	{
		if (rc->sidedistx < rc->sidedisty)
		{
			rc->sidedistx += rc->ddistx;
			rc->mapx += rc->stepx;
			rc->side = 0;
		}
		else
		{
			rc->sidedisty += rc->ddisty;
			rc->mapy += rc->stepy;
			rc->side = 1;
		}
		if (conf->map[rc->mapy][rc->mapx] == '1')
			rc->hit = 1;
	}
}

void		rc_line(t_rc *rc, t_conf *conf)
{
	if (rc->side == 0)
	{
		rc->prpwldist = (rc->mapx - conf->posx + (1 - rc->stepx) / 2);
		rc->prpwldist /= rc->raydirx;
	}
	else
	{
		rc->prpwldist = (rc->mapy - conf->posy + (1 - rc->stepy) / 2);
		rc->prpwldist /= rc->raydiry;
	}
	rc->linehgt = (int)(conf->height / rc->prpwldist);
	rc->drwstrt = (-rc->linehgt + conf->height) / 2;
	if (rc->drwstrt < 0)
		rc->drwstrt = 0;
	rc->drwend = (rc->linehgt + conf->height) / 2;
	if (rc->drwend >= conf->height)
		rc->drwend = conf->height - 1;
}

void		draw_fl_and_ceil(t_rc *rc, t_conf *conf)
{
	int y;

	y = 0;
	while (y < rc->drwstrt)
	{
		my_mlx_pixel_put(&conf->data, rc->x, y, conf->ceil_col);
		y++;
	}
	y = rc->drwend;
	while (y < conf->height)
	{
		my_mlx_pixel_put(&conf->data, rc->x, y, conf->floor_col);
		y++;
	}
}

void		raycasting(t_conf *conf)
{
	double	*zbuff;
	t_rc	rc;

	rc.x = 0;
	zbuff = (double*)malloc(sizeof(double) * conf->width);
	while (rc.x < conf->width)
	{
		rc.camerax = 2 * rc.x / (double)conf->width - 1;
		rc.raydirx = conf->dirx + conf->plx * rc.camerax;
		rc.raydiry = conf->diry + conf->ply * rc.camerax;
		rc_init(&rc, conf);
		rc_dda(&rc, conf);
		rc_line(&rc, conf);
		rc_texx(&rc, conf, conf->tex);
		rc_texstep(&rc, conf->tex);
		rc_walls(&rc, conf, conf->tex);
		draw_fl_and_ceil(&rc, conf);
		zbuff[rc.x] = rc.prpwldist;
		rc.x++;
	}
	spritecast(conf, zbuff, conf->tex[4]);
	free(zbuff);
}
