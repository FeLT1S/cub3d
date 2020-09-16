/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mv.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 19:44:49 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/16 20:54:42 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		left_and_right(t_game *game)
{
	double olddirx;
	double oldplx;
	
  	if (game->left)
	{
      olddirx = game->conf.dirx;
      game->conf.dirx = game->conf.dirx * cos(-rotSpeed)
	  - game->conf.diry * sin(-rotSpeed);
      game->conf.diry = olddirx * sin(-rotSpeed)
	 + game->conf.diry * cos(-rotSpeed);
      oldplx = game->conf.plx;
      game->conf.plx = game->conf.plx * cos(-rotSpeed) - game->conf.ply * sin(-rotSpeed);
      game->conf.ply = oldplx * sin(-rotSpeed) + game->conf.ply * cos(-rotSpeed);
	}
	if (game->right)
	{
      olddirx = game->conf.dirx;
      game->conf.dirx = game->conf.dirx * cos(rotSpeed) - game->conf.diry * sin(rotSpeed);
      game->conf.diry = olddirx * sin(rotSpeed) + game->conf.diry * cos(rotSpeed);
      oldplx = game->conf.plx;
      game->conf.plx = game->conf.plx * cos(rotSpeed) - game->conf.ply * sin(rotSpeed);
      game->conf.ply = oldplx * sin(rotSpeed) + game->conf.ply * cos(rotSpeed);
	}
}

void		check_state(t_game *game)
{
	left_and_right(game);
	if (game->w)
	{
		if (game->conf.map[(int)(game->conf.posy)][(int)(game->conf.posx + game->conf.dirx * moveSpeed)] == '0') 
			game->conf.posx += game->conf.dirx * moveSpeed;
		if (game->conf.map[(int)(game->conf.posy + game->conf.diry * moveSpeed)][(int)(game->conf.posx)] == '0') 
			game->conf.posy += game->conf.diry * moveSpeed;
	}
	if (game->s)
	{
		if (game->conf.map[(int)game->conf.posy][(int)(game->conf.posx - game->conf.dirx * moveSpeed)] == '0') 
			game->conf.posx -= game->conf.dirx * moveSpeed;
		if (game->conf.map[(int)(game->conf.posy - game->conf.diry * moveSpeed)][(int)(game->conf.posx)] == '0') 
			game->conf.posy -= game->conf.diry * moveSpeed;
	}
  	if (game->d)
	{
		if (game->conf.map[(int)(game->conf.posy)][(int)(game->conf.posx - game->conf.diry * moveSpeed)] == '0') 
			game->conf.posx -= game->conf.diry * moveSpeed * 0.5;
		if (game->conf.map[(int)(game->conf.posy + game->conf.dirx * moveSpeed)][(int)(game->conf.posx)] == '0') 
			game->conf.posy += game->conf.dirx * moveSpeed * 0.5;
	}
    if (game->a)
	{
    	if (game->conf.map[(int)(game->conf.posy)][(int)(game->conf.posx + game->conf.diry * moveSpeed)] == '0') 
			game->conf.posx += game->conf.diry * moveSpeed * 0.5;
		if (game->conf.map[(int)(game->conf.posy - game->conf.dirx * moveSpeed)][(int)(game->conf.posx)] == '0') 
			game->conf.posy -= game->conf.dirx * moveSpeed * 0.5;
	}
}
