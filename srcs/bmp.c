/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:17:14 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/26 16:49:47 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <fcntl.h>

void scrin_bmp2(t_conf *conf, int32_t num, char *bmp, uint32_t size)
{
	num = 1;
	ft_memcpy(&bmp[26], &num, 2);
	ft_memcpy(&bmp[28], &conf->data.bpp, 2);
	ft_memcpy(&bmp[54], conf->data.addr, (size - 54));
	num = open("screenshot.bmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	(write(num, bmp, size));
	free(bmp);
	close(num);
	exit(0);
}

void scrin_bmp(t_conf *cfg)
{
	char *bmp;
	int32_t num;
	uint32_t size;

	size = cfg->height * cfg->width *
	(cfg->data.bpp >> 3) + 54;
	if (!(bmp = (char*)malloc(size)))
		ft_error(cfg, 0, 0, 0);
	ft_bzero(bmp, size);
	ft_memcpy(&bmp[0], "BM", 2);
	ft_memcpy(&bmp[2], &size, 4);
	num = 54;
	ft_memcpy(&bmp[10], &num, 4);
	num = 40;
	ft_memcpy(&bmp[14], &num, 4);
	num = cfg->width;
	ft_memcpy(&bmp[18], &num, 4);
	num = -(cfg->height);
	ft_memcpy(&bmp[22], &num, 4);
	scrin_bmp2(cfg, num, bmp, size);
}
void
	ft_bitmap(t_conf *cfg)
{
	while (cfg->width % 64 != 0)
		cfg->width--;
	cfg->data.img = mlx_new_image(cfg->mlx,
	cfg->width, cfg->height);
	cfg->data.addr = mlx_get_data_addr(cfg->data.img,
	&cfg->data.bpp, &cfg->data.l_ln, &cfg->data.enan);
	tex_init(cfg->tex, cfg);
	raycasting(cfg);
	scrin_bmp(cfg);
}
