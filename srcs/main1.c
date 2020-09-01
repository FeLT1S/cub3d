/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 17:18:58 by jiandre           #+#    #+#             */
/*   Updated: 2020/08/31 17:09:37 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mlx.h"

typedef struct s_fff
{
	void *mlx;
	void *win;
	void *image;
	int width;
} t_fff;

int key_event(int keycode, t_fff *mlx)
{
	// (void)param;
	// if (keycode == 53)
	// 	exit(0);
	// return(0);
	if (keycode == 53)
	{
	mlx_clear_window(mlx->mlx, mlx->win);
	int width, height;
	int bpp, size_line, endian;
	mlx->image = mlx_xpm_file_to_image(mlx->mlx, "kofe.xpm", &width, &height);
	//mlx->image = 0x00ffaa00;
	//int img = mlx->image;
	//mlx->image = mlx_new_image(mlx->mlx, 1400, 1200);
	int *data = (int *)mlx_get_data_addr(mlx->image, &bpp, &size_line, &endian);
	for (int i = height * width/4; i < height * width/2; i++)
	 	data[i] = 0x00ffaa00;
	//argb; a - alpha; r - red; g - green; b - blue;
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image, 0, 0);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image, width + (mlx->width += 5), height);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image, width + (mlx->width += 5), 0);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image, 0, height);
	int width2;
	mlx->image = mlx_xpm_file_to_image(mlx->mlx, "danya.xpm",  &width2, &height + 1);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image, width * 2 + (mlx->width += 5), 0);
	mlx_hook(mlx->win, 2, 0, &key_event, mlx);
	}
	return (0);
}

int ft_close(t_fff *mlx)
{
	int width, height;
	int bpp, size_line, endian;
	mlx->image = mlx_xpm_file_to_image(mlx->mlx, "kofe.xpm", &width, &height);
	mlx->win = mlx_new_window(mlx->mlx, height * 4, width * 4, "Danya crazy");
	//mlx->image = mlx_new_image(mlx->mlx, 1400, 1200);
	int *data = (int *)mlx_get_data_addr(mlx->image, &bpp, &size_line, &endian);
	for (int i = height * width/4; i < height * width/2; i++)
	 	data[i] = 0x00ffaa00;
	//argb; a - alpha; r - red; g - green; b - blue;
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image, 0, 0);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image, width, height);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image, width, 0);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image, 0, height);
	int width2;
	mlx->image = mlx_xpm_file_to_image(mlx->mlx, "danya.xpm",  &width2, &height);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image, width *2, 0);
	mlx_hook(mlx->win, 17, 0, &ft_close, mlx);
	mlx_hook(mlx->win, 2, 0, &key_event, mlx);
	mlx_loop(mlx);
	return(0);
}

int		main(void)
{
	t_fff mlxx;

	mlxx.mlx = mlx_init();
	mlxx.win = mlx_new_window(mlxx.mlx, 1400, 1200, "Hello World!");
	mlxx.width = 0;
	mlxx.image = mlx_new_image(mlxx.mlx, 1400, 1200);
	int bpp, size_line, endian;
	int *data = (int *)mlx_get_data_addr(mlxx.image, &bpp, &size_line, &endian);
	for (int i = 0; i < 1400 * 400; i++)
		data[i] = 0x00ffffff;	
	for (int i = 1400* 400; i < 1400 * 800; i++)
		data[i] = 0x00ff0000;
	for (int i = 1400 * 800; i < 1400 * 1200; i++)
		data[i] = 0x00ffffff;
	//argb; a - alpha; r - red; g - green; b - blue;
	mlx_put_image_to_window(mlxx.mlx, mlxx.win, mlxx.image, 0, 0);
	//void *a = NULL;
	mlx_hook(mlxx.win, 17, 0, &ft_close, &mlxx);
	mlx_hook(mlxx.win, 2, 0, &key_event, &mlxx);
	mlx_loop(mlxx.mlx);
	return (0);
}