/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 18:20:30 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/15 21:44:10 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>
#include <stdlib.h>

#define moveSpeed 0.05
#define rotSpeed 0.03

void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->l_ln + x * (data->bpp / 8));
    *(unsigned int*)dst = color;
}

void    sort(int *sprite_order, double *spriteDistance, int amount)
{
  double buff;
  int i;
  int check;
  
  check = 1;
  while (check)
  {
    i = 0;
    check = 0;
    while (i < amount - 1)
    {
      if (spriteDistance[i] < spriteDistance[i + 1])
      {
        check = 1;
        buff = spriteDistance[i];
        spriteDistance[i] = spriteDistance[i + 1];
        spriteDistance[i + 1] = buff;
        buff = sprite_order[i];
        sprite_order[i] = sprite_order[i + 1];
        sprite_order[i + 1] = buff;
      }
      i++;
    }
  }
}

void		rc_init(t_rc *rc, t_conf *conf, int x)
{
	rc->camerax = 2 * x / (double)conf->width - 1;
    rc->raydirx = conf->dirX + conf->planeX * rc->camerax;
    rc->raydiry = conf->dirY + conf->planeY * rc->camerax;
    rc->mapx = (int)conf->posX;
    rc->mapy = (int)conf->posY;
    rc->ddistx = fabs(1 / rc->raydirx);
    rc->ddisty = fabs(1 / rc->raydiry);
    rc->hit = 0;
    if (rc->raydirx < 0)
    {
      rc->stepx = -1;
      rc->sidedistx = (conf->posX - rc->mapx) * rc->ddistx;
    }
    else
    {
		rc->stepx = 1;
    	rc->sidedistx = (rc->mapx + 1.0 - conf->posX) * rc->ddistx;
      }
	if (rc->raydiry < 0)
	{
    	rc->stepy = -1;
        rc->sidedisty = (conf->posY - rc->mapy) * rc->ddisty;
    }
    else
    {
    	rc->stepy = 1;
    	rc->sidedisty = (rc->mapy + 1.0 - conf->posY) * rc->ddisty;
    }
}

void		rc_dda(t_rc *rc, t_conf *conf)
{
	while (rc->hit == 0)
    {
    	if (rc->sidedistx < rc->sidedisty)
    	{
        	rc->sidedistx += rc->ddistx;
        	rc->mapx += rc->stepx;
        	rc->side = 0;
        }
        else
        {
        	rc->sidedisty += rc->ddisty;
        	rc->mapy += rc->stepy;
        	rc->side = 1;
        }
        if (conf->map[rc->mapy][rc->mapx] == '1') 
        	rc->hit = 1;
    }
}

void		rc_line(t_rc *rc, t_conf *conf)
{
	if(rc->side == 0)
	{
		rc->prpwldist = (rc->mapx - conf->posX + (1 - rc->stepx) / 2);
		rc->prpwldist /= rc->raydirx;
	}
    else
	{
		rc->prpwldist = (rc->mapy - conf->posY + (1 - rc->stepy) / 2);
		rc->prpwldist /= rc->raydiry;
	}
	rc->linehgt = (int)(conf->height / rc->prpwldist);
	rc->drwstrt = (-rc->linehgt + conf->height) / 2;
	if(rc->drwstrt < 0)
	rc->drwstrt = 0;
	rc->drwend = (rc->linehgt + conf->height) / 2;
	if(rc->drwend >= conf->height)
    	rc->drwend = conf->height - 1;
}

void		rc_texx(t_rc *rc, t_conf *conf, t_tex *tex)
{
	if(rc->side == 0) 
    	rc->wlx = conf->posY + rc->prpwldist * rc->raydiry;
    else
	rc->wlx = conf->posX + rc->prpwldist * rc->raydirx;
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
				rc->texx = rc->texx;
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
				rc->texx = rc->texx;
			}
    	}
}
void		rc_paint_wall(t_rc *rc, t_conf *conf, t_tex tex, int x, int y)
{
	int color;
	int texY;
	

    texY = (int)rc->tx_pos & (tex.hght - 1);
    color = tex.addr[tex.hght * texY + rc->texx];
    my_mlx_pixel_put(&conf->data, x, y, color);
}

void		rc_walls(t_rc *rc, t_conf *conf, t_tex *tx, int x)
{
		int		y;
		
		y = rc->drwstrt;
		rc[0].tx_pos = (rc->drwstrt - (conf->height + rc->linehgt) / 2) * rc->step;
		while (y < rc->drwend)
		{
        	rc->tx_pos += rc->step;
        	if (rc->side == 0)
        	{
          		if (rc->stepx < 0)
		  			rc_paint_wall(rc, conf, tx[0], x, y);
          		else
					rc_paint_wall(rc, conf, tx[1], x, y);
			}
        	else
        	{
				if (rc->stepy < 0)
					rc_paint_wall(rc, conf, tx[2], x, y);
				else
					rc_paint_wall(rc, conf, tx[3], x, y);
			}
			y++;
		}
}

void		tex_init(t_tex *tex, t_conf *conf)
{
	if (!(tex[0].img = mlx_xpm_file_to_image(conf->mlx, conf->we_path, &tex[0].wdth, &tex[0].hght)))
		ft_error(conf, 0, 0, 0);
	if (!(tex[1].img = mlx_xpm_file_to_image(conf->mlx, conf->ea_path, &tex[1].wdth, &tex[1].hght)))
		ft_error(conf, 0, 0, 0);
	if (!(tex[2].img = mlx_xpm_file_to_image(conf->mlx, conf->no_path, &tex[2].wdth, &tex[2].hght)))
		ft_error(conf, 0, 0, 0);
	if (!(tex[3].img = mlx_xpm_file_to_image(conf->mlx, conf->so_path, &tex[3].wdth, &tex[3].hght)))
		ft_error(conf, 0, 0, 0);
	if (!(tex[4].img = mlx_xpm_file_to_image(conf->mlx, conf->s_path, &tex[4].wdth, &tex[4].hght)))
		ft_error(conf, 0, 0, 0);
	tex[0].addr = (int*)mlx_get_data_addr(tex[0].img, &tex[0].bpp, &tex[0].l_ln, &tex[0].enan);
	tex[1].addr = (int*)mlx_get_data_addr(tex[1].img, &tex[1].bpp, &tex[1].l_ln, &tex[1].enan);
	tex[2].addr = (int*)mlx_get_data_addr(tex[2].img, &tex[2].bpp, &tex[2].l_ln, &tex[2].enan);
	tex[3].addr = (int*)mlx_get_data_addr(tex[3].img, &tex[3].bpp, &tex[3].l_ln, &tex[3].enan);
	tex[4].addr = (int*)mlx_get_data_addr(tex[4].img, &tex[4].bpp, &tex[4].l_ln, &tex[4].enan);
}

void	srt_sprites(t_conf *conf, int *sprt_ord, double *sprt_dst)
{
	int i;

	i = 0;
	while ( i < conf->num_sprs)
	{
		sprt_ord[i] = i;
		sprt_dst[i] = (pow((conf->posX - conf->sprite[i].x), 2) + pow((conf->posY - conf->sprite[i].y), 2));
		i++;
	}
	sort(sprt_ord, sprt_dst, conf->num_sprs);
}

void	mtrx_trans(t_conf *conf, t_sc *sc, int *sprt_ord, int i)
{
	sc->x = conf->sprite[sprt_ord[i]].x - conf->posX;
	sc->y = conf->sprite[sprt_ord[i]].y - conf->posY;
	sc->invdet = 1.0 / (conf->planeX * conf->dirY - conf->dirX * conf->planeY);
	sc->transx = sc->invdet * (conf->dirY * sc->x - conf->dirX * sc->y);
	sc->transy = sc->invdet * (-conf->planeY * sc->x + conf->planeX * sc->y);
	sc->screenx= (int)((conf->width / 2) * (1 + sc->transx / sc->transy));
	sc->hght = abs((int)(conf->height / (sc->transy)));
}

void	tex_spr(t_conf *conf, t_sc *sc)
{
	sc->draw_starty = -sc->hght / 2 + conf->height / 2;
	if(sc->draw_starty < 0) 
		sc->draw_starty = 0;
	sc->draw_endy = sc->hght / 2 + conf->height / 2;
	if(sc->draw_endy >= conf->height) 
		sc->draw_endy = conf->height - 1;
	sc->wdth = abs((int)(conf->height / sc->transy));
	sc->draw_startx = -sc->wdth / 2 + sc->screenx;
	if (sc->draw_startx < 0) 
		sc->draw_startx = 0;
	sc->draw_endx = sc->wdth / 2 + sc->screenx;
	if (sc->draw_endx >= conf->width) 
		sc->draw_endx = conf->width - 1;
}

void	draw_sprite(t_sc *sc, t_conf *conf, double *zbuff, t_tex tex)
{
	int x;
	int y;
	int d;
	int color;
	
	x = sc->draw_startx;
	while (x < sc->draw_endx)
    {
		y = sc->draw_starty;
		sc->texx = (int)256 * (x - (-sc->wdth / 2 + sc->screenx));
		sc->texx = sc->texx * tex.wdth / sc->wdth / 256;
		if(sc->transy > 0 && x > 0 && x < conf->width && sc->transy < zbuff[x])
		{
			while (y < sc->draw_endy)
			{
				d = (y) * 256 - conf->height * 128 + sc->hght * 128;
				sc->texy = ((d * tex.hght) / sc->hght) / 256;
				color = tex.addr[tex.wdth * sc->texy + sc->texx];
				if(color != -0x1000000)
					my_mlx_pixel_put(&conf->data, x, y, color);
				y++;
			}
		}
		x++;
	  }
}

void	spritecast(t_conf *conf, double *zbuff, t_tex tex)
{
	t_sc	sc;
	int		sprt_ord[conf->num_sprs];
	double	sprt_dst[conf->num_sprs];
	int i;
	
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

void		raycasting(t_conf *conf)
{
	double zbuff[conf->width];
	t_rc rc;
	t_tex tex[5];
	int x;
	
	x = 0;
	tex_init(tex, conf);
	while (x < conf->width)
    {
		rc_init(&rc, conf, x);
		rc_dda(&rc, conf);
		rc_line(&rc, conf);
		rc_texx(&rc, conf, tex);
		rc_texstep(&rc, tex);
		rc_walls(&rc, conf, tex, x);
		for (int y = 0; y < rc.drwstrt; y++)
			my_mlx_pixel_put(&conf->data, x, y, conf->ceil_col);
		for (int y = rc.drwend; y < conf->height; y++)
			my_mlx_pixel_put(&conf->data, x, y, conf->floor_col);
    	zbuff[x] = rc.prpwldist;
		x++;
    }
	spritecast(conf, zbuff, tex[4]);
}

void		check_state(t_game *game)
{
	if (game->w)
	{
		if(game->conf.map[(int)(game->conf.posY)][(int)(game->conf.posX + game->conf.dirX * moveSpeed)] == '0') game->conf.posX += game->conf.dirX * moveSpeed;
		if(game->conf.map[(int)(game->conf.posY + game->conf.dirY * moveSpeed)][(int)(game->conf.posX)] == '0') game->conf.posY += game->conf.dirY * moveSpeed;
	}
	if (game->s)
	{
		if(game->conf.map[(int)game->conf.posY][(int)(game->conf.posX - game->conf.dirX * moveSpeed)] == '0') game->conf.posX -= game->conf.dirX * moveSpeed;
		if(game->conf.map[(int)(game->conf.posY - game->conf.dirY * moveSpeed)][(int)(game->conf.posX)] == '0') game->conf.posY -= game->conf.dirY * moveSpeed;
	}
  	if (game->left)
	{
      double olddirX = game->conf.dirX;
      game->conf.dirX = game->conf.dirX * cos(-rotSpeed) - game->conf.dirY * sin(-rotSpeed);
      game->conf.dirY = olddirX * sin(-rotSpeed) + game->conf.dirY * cos(-rotSpeed);
      double oldplaneX = game->conf.planeX;
      game->conf.planeX = game->conf.planeX * cos(-rotSpeed) - game->conf.planeY * sin(-rotSpeed);
      game->conf.planeY = oldplaneX * sin(-rotSpeed) + game->conf.planeY * cos(-rotSpeed);
	}
	if (game->right)
	{
      double olddirX = game->conf.dirX;
      game->conf.dirX = game->conf.dirX * cos(rotSpeed) - game->conf.dirY * sin(rotSpeed);
      game->conf.dirY = olddirX * sin(rotSpeed) + game->conf.dirY * cos(rotSpeed);
      double oldplaneX = game->conf.planeX;
      game->conf.planeX = game->conf.planeX * cos(rotSpeed) - game->conf.planeY * sin(rotSpeed);
      game->conf.planeY = oldplaneX * sin(rotSpeed) + game->conf.planeY * cos(rotSpeed);
	}
  	if (game->d)
	{
		if(game->conf.map[(int)(game->conf.posY)][(int)(game->conf.posX - game->conf.dirY * moveSpeed)] == '0') game->conf.posX -= game->conf.dirY * moveSpeed * 0.5;
		if(game->conf.map[(int)(game->conf.posY + game->conf.dirX * moveSpeed)][(int)(game->conf.posX)] == '0') game->conf.posY += game->conf.dirX * moveSpeed * 0.5;
	}
    if (game->a)
	{
    if(game->conf.map[(int)(game->conf.posY)][(int)(game->conf.posX + game->conf.dirY * moveSpeed)] == '0') game->conf.posX += game->conf.dirY * moveSpeed * 0.5;
		if(game->conf.map[(int)(game->conf.posY - game->conf.dirX * moveSpeed)][(int)(game->conf.posX)] == '0') game->conf.posY -= game->conf.dirX * moveSpeed * 0.5;
	}
}

int			draw_frame(t_game *game)
{
  if (!(game->w || game->left || game->right || game->a || game->s || game->d))
    return (0);
	check_state(game);
	game->conf.data.img = mlx_new_image(game->conf.mlx, game->conf.width, game->conf.height);
	game->conf.data.addr =  mlx_get_data_addr(game->conf.data.img, &game->conf.data.bpp, &game->conf.data.l_ln, &game->conf.data.enan);
	raycasting(&game->conf);
	mlx_put_image_to_window(game->conf.mlx, game->win, game->conf.data.img, 0, 0);
	mlx_destroy_image(game->conf.mlx, game->conf.data.img);
	return (0);
}

int key_press(int key, t_game *game)
{
  if (key == 53)
    exit(0);
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

int key_unpress(int key, t_game *game)
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

void  init(t_game *game)
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

int main(int argc, char **argv)
{
  t_game game;

  init(&game);
  if (argc > 3)
    return (0);
  chk_tag(&game.conf, argv[1], ".cub");
	ft_parsing(argv[1], &game.conf);
	game.conf.mlx = mlx_init();
	game.win = mlx_new_window(game.conf.mlx, game.conf.width, game.conf.height, "Cub3d");
  check_state(&game);
	game.conf.data.img = mlx_new_image(game.conf.mlx, game.conf.width, game.conf.height);
	game.conf.data.addr =  mlx_get_data_addr(game.conf.data.img, &game.conf.data.bpp, &game.conf.data.l_ln, &game.conf.data.enan);
	raycasting(&game.conf);
	mlx_put_image_to_window(game.conf.mlx, game.win, game.conf.data.img, 0, 0);
	mlx_destroy_image(game.conf.mlx, game.conf.data.img);
	mlx_loop_hook(game.conf.mlx, draw_frame, &game);
	mlx_hook(game.win, 2, (1L<<0), key_press, &game);
	mlx_hook(game.win, 3, (1L<<1), key_unpress, &game);
	mlx_loop(game.conf.mlx);
}