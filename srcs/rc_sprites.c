/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc_sprites.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 19:33:26 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/16 20:20:00 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		srt_sprites(t_conf *conf, int *sprt_ord, double *sprt_dst)
{
	int		i;

	i = 0;
	while (i < conf->num_sprs)
	{
		sprt_ord[i] = i;
		sprt_dst[i] = pow((conf->posx - conf->sprite[i].x), 2);
		sprt_dst[i] += pow((conf->posy - conf->sprite[i].y), 2);
		i++;
	}
	sort(sprt_ord, sprt_dst, conf->num_sprs);
}

void		mtrx_trans(t_conf *conf, t_sc *sc, int *sprt_ord, int i)
{
	sc->x = conf->sprite[sprt_ord[i]].x - conf->posx;
	sc->y = conf->sprite[sprt_ord[i]].y - conf->posy;
	sc->invdet = 1.0 / (conf->plx * conf->diry - conf->dirx * conf->ply);
	sc->transx = sc->invdet * (conf->diry * sc->x - conf->dirx * sc->y);
	sc->transy = sc->invdet * (-conf->ply * sc->x + conf->plx * sc->y);
	sc->screenx = (int)((conf->width / 2) * (1 + sc->transx / sc->transy));
	sc->hght = abs((int)(conf->height / (sc->transy)));
}

void		tex_spr(t_conf *conf, t_sc *sc)
{
	sc->draw_starty = -sc->hght / 2 + conf->height / 2;
	if (sc->draw_starty < 0)
		sc->draw_starty = 0;
	sc->draw_endy = sc->hght / 2 + conf->height / 2;
	if (sc->draw_endy >= conf->height)
		sc->draw_endy = conf->height - 1;
	sc->wdth = abs((int)(conf->height / sc->transy));
	sc->draw_startx = -sc->wdth / 2 + sc->screenx;
	if (sc->draw_startx < 0)
		sc->draw_startx = 0;
	sc->draw_endx = sc->wdth / 2 + sc->screenx;
	if (sc->draw_endx >= conf->width)
		sc->draw_endx = conf->width - 1;
}

void		draw_sprite(t_sc *sc, t_conf *conf, double *zbuff, t_tex tex)
{
	int		x;
	int		y;
	int		d;
	int		color;

	x = sc->draw_startx;
	while (x < sc->draw_endx)
	{
		y = sc->draw_starty;
		sc->texx = (int)256 * (x - (-sc->wdth / 2 + sc->screenx));
		sc->texx = sc->texx * tex.wdth / sc->wdth / 256;
		if (sc->transy > 0 && x > 0 && x < conf->width && sc->transy < zbuff[x])
		{
			while (y < sc->draw_endy)
			{
				d = (y) * 256 - conf->height * 128 + sc->hght * 128;
				sc->texy = ((d * tex.hght) / sc->hght) / 256;
				color = tex.addr[tex.wdth * sc->texy + sc->texx];
				if (color != -0x1000000)
					my_mlx_pixel_put(&conf->data, x, y, color);
				y++;
			}
		}
		x++;
	}
}

void		spritecast(t_conf *conf, double *zbuff, t_tex tex)
{
	t_sc	sc;
	int		sprt_ord[conf->num_sprs];
	double	sprt_dst[conf->num_sprs];
	int		i;

	i = 0;
	srt_sprites(conf, sprt_ord, sprt_dst);
	while (i < conf->num_sprs)
	{
		mtrx_trans(conf, &sc, sprt_ord, i);
		tex_spr(conf, &sc);
		draw_sprite(&sc, conf, zbuff, tex);
		i++;
	}
}
