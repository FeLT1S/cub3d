/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mv.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 19:44:49 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/18 17:21:58 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		right(t_game *game)
{
	double	olddirx;
	double	oldplx;

	olddirx = game->conf.dirx;
	game->conf.dirx = game->conf.dirx * cos(RTSPD)
	- game->conf.diry * sin(RTSPD);
	game->conf.diry = olddirx * sin(RTSPD) + game->conf.diry * cos(RTSPD);
	oldplx = game->conf.plx;
	game->conf.plx = game->conf.plx * cos(RTSPD)
	- game->conf.ply * sin(RTSPD);
	game->conf.ply = oldplx * sin(RTSPD) + game->conf.ply * cos(RTSPD);
}

void		left(t_game *game)
{
	double	olddirx;
	double	oldplx;

	olddirx = game->conf.dirx;
	game->conf.dirx = game->conf.dirx * cos(-RTSPD)
	- game->conf.diry * sin(-RTSPD);
	game->conf.diry = olddirx * sin(-RTSPD)
	+ game->conf.diry * cos(-RTSPD);
	oldplx = game->conf.plx;
	game->conf.plx = game->conf.plx * cos(-RTSPD)
	- game->conf.ply * sin(-RTSPD);
	game->conf.ply = oldplx * sin(-RTSPD) + game->conf.ply * cos(-RTSPD);
}

void		forward_back(t_game *game)
{
	double	posx;
	double	posy;

	if (game->w)
	{
		posx = game->conf.posx + game->conf.dirx * MVSPD;
		posy = game->conf.posy + game->conf.diry * MVSPD;
		if (game->conf.map[(int)(game->conf.posy)][(int)(posx + 0.01)] == '0' &&
		game->conf.map[(int)(game->conf.posy)][(int)(posx - 0.01)] == '0')
			game->conf.posx += game->conf.dirx * MVSPD;
		if (game->conf.map[(int)(posy + 0.01)][(int)(game->conf.posx)] == '0' &&
		game->conf.map[(int)(posy - 0.01)][(int)(game->conf.posx)] == '0')
			game->conf.posy += game->conf.diry * MVSPD;
	}
	if (game->s)
	{
		posx = game->conf.posx - game->conf.dirx * MVSPD;
		posy = game->conf.posy - game->conf.diry * MVSPD;
		if (game->conf.map[(int)(game->conf.posy)][(int)(posx + 0.01)] == '0' &&
		game->conf.map[(int)(game->conf.posy)][(int)(posx - 0.01)] == '0')
			game->conf.posx -= game->conf.dirx * MVSPD;
		if (game->conf.map[(int)(posy + 0.01)][(int)(game->conf.posx)] == '0' &&
		game->conf.map[(int)(posy - 0.01)][(int)(game->conf.posx)] == '0')
			game->conf.posy -= game->conf.diry * MVSPD;
	}
}

void		left_and_right(t_game *game)
{
	double	posx;
	double	posy;

	if (game->d)
	{
		posx = game->conf.posx - game->conf.diry * MVSPD;
		posy = game->conf.posy + game->conf.dirx * MVSPD;
		if (game->conf.map[(int)(game->conf.posy)][(int)(posx + 0.01)] == '0' &&
		game->conf.map[(int)(game->conf.posy)][(int)(posx - 0.01)] == '0')
			game->conf.posx -= game->conf.diry * MVSPD;
		if (game->conf.map[(int)(posy + 0.01)][(int)(game->conf.posx)] == '0' &&
		game->conf.map[(int)(posy - 0.01)][(int)(game->conf.posx)] == '0')
			game->conf.posy += game->conf.dirx * MVSPD;
	}
	if (game->a)
	{
		posx = game->conf.posx + game->conf.diry * MVSPD;
		posy = game->conf.posy - game->conf.dirx * MVSPD;
		if (game->conf.map[(int)(game->conf.posy)][(int)(posx + 0.1)] == '0' &&
		game->conf.map[(int)(game->conf.posy)][(int)(posx - 0.1)] == '0')
			game->conf.posx += game->conf.diry * MVSPD;
		if (game->conf.map[(int)(posy + 0.1)][(int)(game->conf.posx)] == '0' &&
		game->conf.map[(int)(posy - 0.1)][(int)(game->conf.posx)] == '0')
			game->conf.posy -= game->conf.dirx * MVSPD;
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
