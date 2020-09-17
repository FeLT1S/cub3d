/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mv.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 19:44:49 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/17 18:45:37 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		right(t_game *game)
{
	double	olddirx;
	double	oldplx;

	olddirx = game->conf.dirx;
	game->conf.dirx = game->conf.dirx * cos(rtspd)
	- game->conf.diry * sin(rtspd);
	game->conf.diry = olddirx * sin(rtspd) + game->conf.diry * cos(rtspd);
	oldplx = game->conf.plx;
	game->conf.plx = game->conf.plx * cos(rtspd)
	- game->conf.ply * sin(rtspd);
	game->conf.ply = oldplx * sin(rtspd) + game->conf.ply * cos(rtspd);
}

void		left(t_game *game)
{
	double	olddirx;
	double	oldplx;

	olddirx = game->conf.dirx;
	game->conf.dirx = game->conf.dirx * cos(-rtspd)
	- game->conf.diry * sin(-rtspd);
	game->conf.diry = olddirx * sin(-rtspd)
	+ game->conf.diry * cos(-rtspd);
	oldplx = game->conf.plx;
	game->conf.plx = game->conf.plx * cos(-rtspd)
	- game->conf.ply * sin(-rtspd);
	game->conf.ply = oldplx * sin(-rtspd) + game->conf.ply * cos(-rtspd);
}

void		forward_back(t_game *game)
{
	double	posx;
	double	posy;

	if (game->w)
	{
		posx = game->conf.posx + game->conf.dirx * mvspd;
		posy = game->conf.posy + game->conf.diry * mvspd;
		if (game->conf.map[(int)(game->conf.posy)][(int)(posx + 0.01)] == '0' &&
		game->conf.map[(int)(game->conf.posy)][(int)(posx - 0.01)] == '0')
			game->conf.posx += game->conf.dirx * mvspd;
		if (game->conf.map[(int)(posy + 0.01)][(int)(game->conf.posx)] == '0' &&
		game->conf.map[(int)(posy - 0.01)][(int)(game->conf.posx)] == '0')
			game->conf.posy += game->conf.diry * mvspd;
	}
	if (game->s)
	{
		posx = game->conf.posx - game->conf.dirx * mvspd;
		posy = game->conf.posy - game->conf.diry * mvspd;
		if (game->conf.map[(int)(game->conf.posy)][(int)(posx + 0.01)] == '0' &&
		game->conf.map[(int)(game->conf.posy)][(int)(posx - 0.01)] == '0')
			game->conf.posx -= game->conf.dirx * mvspd;
		if (game->conf.map[(int)(posy + 0.01)][(int)(game->conf.posx)] == '0' &&
		game->conf.map[(int)(posy - 0.01)][(int)(game->conf.posx)] == '0')
			game->conf.posy -= game->conf.diry * mvspd;
	}
}

void		left_and_right(t_game *game)
{
	double	posx;
	double	posy;

	if (game->d)
	{
		posx = game->conf.posx - game->conf.diry * mvspd;
		posy = game->conf.posy + game->conf.dirx * mvspd;
		if (game->conf.map[(int)(game->conf.posy)][(int)(posx + 0.01)] == '0' &&
		game->conf.map[(int)(game->conf.posy)][(int)(posx - 0.01)] == '0')
			game->conf.posx -= game->conf.diry * mvspd;
		if (game->conf.map[(int)(posy + 0.01)][(int)(game->conf.posx)] == '0' &&
		game->conf.map[(int)(posy - 0.01)][(int)(game->conf.posx)] == '0')
			game->conf.posy += game->conf.dirx * mvspd;
	}
	if (game->a)
	{
		posx = game->conf.posx + game->conf.diry * mvspd;
		posy = game->conf.posy - game->conf.dirx * mvspd;
		if (game->conf.map[(int)(game->conf.posy)][(int)(posx + 0.1)] == '0' &&
		game->conf.map[(int)(game->conf.posy)][(int)(posx - 0.1)] == '0')
			game->conf.posx += game->conf.diry * mvspd;
		if (game->conf.map[(int)(posy + 0.1)][(int)(game->conf.posx)] == '0' &&
		game->conf.map[(int)(posy - 0.1)][(int)(game->conf.posx)] == '0')
			game->conf.posy -= game->conf.dirx * mvspd;
	}
}

void		check_state(t_game *game)
{
	if (game->left)
		left(game);
	if (game->right)
		right(game);
	if (game->w || game->s)
		forward_back(game);
	if (game->a || game->d)
		left_and_right(game);
}
