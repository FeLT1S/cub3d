/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:17:14 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/18 17:08:16 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <fcntl.h>

static void
	set_int_in_char(unsigned char *start, int value)
{
	start[0] = (unsigned char)(value);
	start[1] = (unsigned char)(value >> 8);
	start[2] = (unsigned char)(value >> 16);
	start[3] = (unsigned char)(value >> 24);
}

static int
	write_bmp_header(int fd, int filesize, t_conf *cfg)
{
	int				i;
	int				tmp;
	unsigned char	bmpfileheader[54];

	i = 0;
	while (i < 54)
		bmpfileheader[i++] = (unsigned char)(0);
	bmpfileheader[0] = (unsigned char)('B');
	bmpfileheader[1] = (unsigned char)('M');
	set_int_in_char(bmpfileheader + 2, filesize);
	bmpfileheader[10] = (unsigned char)(54);
	bmpfileheader[14] = (unsigned char)(40);
	tmp = cfg->width;
	set_int_in_char(bmpfileheader + 18, tmp);
	tmp = cfg->height;
	set_int_in_char(bmpfileheader + 22, tmp);
	bmpfileheader[27] = (unsigned char)(1);
	bmpfileheader[28] = (unsigned char)(24);
	return (!(write(fd, bmpfileheader, 54) < 0));
}

static int
	get_color(t_conf *cfg, int x, int y)
{
	int	rgb;
	int	color;

	color = *(int*)(cfg->data.addr
			+ (4 * cfg->width * ((int)cfg->height - 1 - y))
			+ (4 * x));
	rgb = (color & 0xFF0000) | (color & 0x00FF00) | (color & 0x0000FF);
	return (rgb);
}

static int
	write_bmp_data(int file, t_conf *cfg, int pad)
{
	const unsigned char	zero[3] = {0, 0, 0};
	int					i;
	int					j;
	int					color;

	i = 0;
	while (i < (int)cfg->height)
	{
		j = 0;
		while (j < (int)cfg->width)
		{
			color = get_color(cfg, j, i);
			if (write(file, &color, 3) < 0)
				return (0);
			if (pad > 0 && write(file, &zero, pad) < 0)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void
	ft_bitmap(t_conf *cfg)
{
	int			filesize;
	int			file;
	int			pad;

	while (cfg->width % 64 != 0)
		cfg->width--;
	cfg->data.img = mlx_new_image(cfg->mlx,
	cfg->width, cfg->height);
	cfg->data.addr = mlx_get_data_addr(cfg->data.img,
	&cfg->data.bpp, &cfg->data.l_ln, &cfg->data.enan);
	tex_init(cfg->tex, cfg);
	raycasting(cfg);
	pad = (4 - ((int)cfg->width * 3) % 4) % 4;
	filesize = 54 + (3 * ((int)cfg->width + pad) * (int)cfg->height);
	if ((file = open("screenshot.bmp",
		O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU)) < 0)
		ft_error(cfg, 0, 0, 0);
	if (!write_bmp_header(file, filesize, cfg))
		ft_error(cfg, 0, 0, 0);
	write_bmp_data(file, cfg, pad);
	close(file);
	ft_close(cfg);
}
