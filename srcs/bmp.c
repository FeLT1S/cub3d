/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 19:17:14 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/17 21:05:41 by jiandre          ###   ########.fr       */
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
	write_bmp_header(int fd, int filesize, t_game *game)
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
	tmp = game->conf.width;
	set_int_in_char(bmpfileheader + 18, tmp);
	tmp = game->conf.height;
	set_int_in_char(bmpfileheader + 22, tmp);
	bmpfileheader[27] = (unsigned char)(1);
	bmpfileheader[28] = (unsigned char)(24);
	return (!(write(fd, bmpfileheader, 54) < 0));
}

void	ft_bdata(t_game *s, int fd)
{
	int				i;
	int				j;
	unsigned char	buffer[4];

	i = s->conf.width * (s->conf.height - 1);
	while (i >= 0)
	{
		j = 0;
		while (j < s->conf.width)
		{
			buffer[0] = (unsigned char)(s->conf.data.addr[i] % 256);
			buffer[1] = (unsigned char)(s->conf.data.addr[i] / 256 % 256);
			buffer[2] = (unsigned char)(s->conf.data.addr[i] / 256 / 256 % 256);
			buffer[3] = (unsigned char)(0);
			write(fd, buffer, 4);
			i++;
			j++;
		}
		i -= 4;
	}
}

static int
	get_color(t_game *game, int x, int y)
{
	int	rgb;
	int	color;

	color = *(int*)(game->conf.data.addr
			+ (4 * game->conf.width * ((int)game->conf.height - 1 - y))
			+ (4 * x));
	rgb = (color & 0xFF0000) | (color & 0x00FF00) | (color & 0x0000FF);
	return (rgb);
}

static int
	write_bmp_data(int file, t_game *game, int pad)
{
	const unsigned char	zero[3] = {0, 0, 0};
	int					i;
	int					j;
	int					color;

	i = 0;
	while (i < (int)game->conf.height)
	{
		j = 0;
		while (j < (int)game->conf.width)
		{
			color = get_color(game, j, i);
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

int
	save_bmp(t_game *game)
{
	int			filesize;
	int			file;
	int			pad;

	pad = (4 - ((int)game->conf.width * 3) % 4) % 4;
	filesize = 54 + (3 * ((int)game->conf.width + pad) * (int)game->conf.height);
	if ((file = open("screenshot.bmp", O_WRONLY | O_CREAT
									| O_TRUNC | O_APPEND)) < 0)
		return (0);
	if (!write_bmp_header(file, filesize, game))
		return (0);
	write_bmp_data(file, game, pad);
	close(file);
	return (1);
}