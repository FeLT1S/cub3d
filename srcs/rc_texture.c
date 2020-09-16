/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc_texture.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 19:32:45 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/16 20:14:51 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		rc_texx(t_rc *rc, t_conf *conf, t_tex *tex)
{
	if (rc->side == 0)
		rc->wlx = conf->posy + rc->prpwldist * rc->raydiry;
	else
		rc->wlx = conf->posx + rc->prpwldist * rc->raydirx;
	rc->wlx -= (int)rc->wlx;
	if (rc->side == 0)
	{
		if (rc->stepx < 0)
			rc->texx = (int)(rc->wlx * (double)(tex[0].wdth));
		else
			rc->texx = (int)(rc->wlx * (double)(tex[1].wdth));
	}
	else
	{
		if (rc->stepy < 0)
			rc->texx = (int)(rc->wlx * (double)(tex[2].wdth));
		else
			rc->texx = (int)(rc->wlx * (double)(tex[3].wdth));
	}
}

void		rc_texstep(t_rc *rc, t_tex *tex)
{
	if (rc->side == 0)
	{
		if (rc->stepx < 0)
		{
			rc->step = 1.0 * tex[0].hght / rc->linehgt;
			rc->texx = tex->hght - rc->texx - 1;
		}
		else
			rc->step = 1.0 * tex[1].hght / rc->linehgt;
	}
	else
	{
		if (rc->stepy < 0)
			rc->step = 1.0 * tex[2].hght / rc->linehgt;
		else
		{
			rc->step = 1.0 * tex[3].hght / rc->linehgt;
			rc->texx = tex->hght - rc->texx - 1;
		}
	}
}

void		rc_paint_wall(t_rc *rc, t_conf *conf, t_tex tex)
{
	int		color;
	int		texy;

	texy = (int)rc->tx_pos & (tex.hght - 1);
	color = tex.addr[tex.hght * texy + rc->texx];
	my_mlx_pixel_put(&conf->data, rc->x, rc->y, color);
}

void		rc_walls(t_rc *rc, t_conf *conf, t_tex *tx)
{
	rc->y = rc->drwstrt;
	rc[0].tx_pos = (rc->drwstrt - (conf->height + rc->linehgt) / 2) * rc->step;
	while (rc->y < rc->drwend)
	{
		rc->tx_pos += rc->step;
		if (rc->side == 0)
		{
			if (rc->stepx < 0)
				rc_paint_wall(rc, conf, tx[0]);
			else
				rc_paint_wall(rc, conf, tx[1]);
		}
		else
		{
			if (rc->stepy < 0)
				rc_paint_wall(rc, conf, tx[2]);
			else
				rc_paint_wall(rc, conf, tx[3]);
		}
		rc->y++;
	}
}

void		tex_init(t_tex *tex, t_conf *conf)
{
	if (!(tex[0].img = mlx_xpm_file_to_image(conf->mlx,
	conf->we_path, &tex[0].wdth, &tex[0].hght)))
		ft_error(conf, 0, 0, 0);
	if (!(tex[1].img = mlx_xpm_file_to_image(conf->mlx,
	conf->ea_path, &tex[1].wdth, &tex[1].hght)))
		ft_error(conf, 0, 0, 0);
	if (!(tex[2].img = mlx_xpm_file_to_image(conf->mlx,
	conf->no_path, &tex[2].wdth, &tex[2].hght)))
		ft_error(conf, 0, 0, 0);
	if (!(tex[3].img = mlx_xpm_file_to_image(conf->mlx,
	conf->so_path, &tex[3].wdth, &tex[3].hght)))
		ft_error(conf, 0, 0, 0);
	if (!(tex[4].img = mlx_xpm_file_to_image(conf->mlx,
	conf->s_path, &tex[4].wdth, &tex[4].hght)))
		ft_error(conf, 0, 0, 0);
	tex[0].addr = (int*)mlx_get_data_addr(tex[0].img,
	&tex[0].bpp, &tex[0].l_ln, &tex[0].enan);
	tex[1].addr = (int*)mlx_get_data_addr(tex[1].img,
	&tex[1].bpp, &tex[1].l_ln, &tex[1].enan);
	tex[2].addr = (int*)mlx_get_data_addr(tex[2].img,
	&tex[2].bpp, &tex[2].l_ln, &tex[2].enan);
	tex[3].addr = (int*)mlx_get_data_addr(tex[3].img,
	&tex[3].bpp, &tex[3].l_ln, &tex[3].enan);
	tex[4].addr = (int*)mlx_get_data_addr(tex[4].img,
	&tex[4].bpp, &tex[4].l_ln, &tex[4].enan);
}
