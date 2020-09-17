/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 18:20:30 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/17 20:27:04 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int			draw_frame(t_game *game)
{
	if (!(game->w || game->left || game->right
	|| game->a || game->s || game->d))
		return (0);
	check_state(game);
	game->conf.data.img = mlx_new_image(game->conf.mlx,
	game->conf.width, game->conf.height);
	game->conf.data.addr = mlx_get_data_addr(game->conf.data.img,
	&game->conf.data.bpp, &game->conf.data.l_ln, &game->conf.data.enan);
	raycasting(&game->conf);
	mlx_put_image_to_window(game->conf.mlx,
	game->win, game->conf.data.img, 0, 0);
	mlx_destroy_image(game->conf.mlx, game->conf.data.img);
	return (0);
}

int			key_press(int key, t_game *game)
{
	if (key == 53)
		ft_close(&game->conf);
	if (key == kVK_ANSI_W)
		game->w = 1;
	if (key == kVK_ANSI_S)
		game->s = 1;
	if (key == kVK_LeftArrow)
		game->left = 1;
	if (key == kVK_RightArrow)
		game->right = 1;
	if (key == kVK_ANSI_A)
		game->a = 1;
	if (key == kVK_ANSI_D)
		game->d = 1;
	return (0);
}

int			key_unpress(int key, t_game *game)
{
	if (key == kVK_ANSI_W)
		game->w = 0;
	if (key == kVK_ANSI_S)
		game->s = 0;
	if (key == kVK_LeftArrow)
		game->left = 0;
	if (key == kVK_RightArrow)
		game->right = 0;
	if (key == kVK_ANSI_A)
		game->a = 0;
	if (key == kVK_ANSI_D)
		game->d = 0;
	return (0);
}

void		init(t_game *game)
{
	game->w = 0;
	game->s = 0;
	game->a = 0;
	game->d = 0;
	game->left = 0;
	game->right = 0;
	game->conf.num_sprs = 0;
	game->conf.no_path = 0;
	game->conf.so_path = 0;
	game->conf.ea_path = 0;
	game->conf.we_path = 0;
	game->conf.s_path = 0;
	game->conf.map = 0;
	game->conf.sprite = 0;
}

int			main(int argc, char **argv)
{
	t_game	game;

	init(&game);
	if (argc > 3)
		return (0);
	chk_tag(&game.conf, argv[1], ".cub");
	ft_parsing(argv[1], &game.conf);
	game.conf.mlx = mlx_init();
	game.win = mlx_new_window(game.conf.mlx,
	game.conf.width, game.conf.height, "Cub3d");
	check_state(&game);
	game.conf.data.img = mlx_new_image(game.conf.mlx,
	game.conf.width, game.conf.height);
	game.conf.data.addr = mlx_get_data_addr(game.conf.data.img,
	&game.conf.data.bpp, &game.conf.data.l_ln, &game.conf.data.enan);
	tex_init(game.conf.tex, &game.conf);
	raycasting(&game.conf);
	save_bmp(&game);
	mlx_put_image_to_window(game.conf.mlx,
	game.win, game.conf.data.img, 0, 0);
	mlx_destroy_image(game.conf.mlx, game.conf.data.img);
	mlx_loop_hook(game.conf.mlx, draw_frame, &game);
	mlx_hook(game.win, 2, (1L << 0), key_press, &game);
	mlx_hook(game.win, 17, 0, ft_close, &game.conf);
	mlx_hook(game.win, 3, (1L << 1), key_unpress, &game);
	mlx_loop(game.conf.mlx);
}
