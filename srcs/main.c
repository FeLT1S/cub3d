/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 18:20:30 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/03 20:52:07 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "cub3d.h"
#include <math.h>
#include <stdio.h>

// #define screenWidth 1280
// #define screenHeight 1024
#define mapWidth 24
#define mapHeight 24
#define moveSpeed 0.05
#define rotSpeed 0.03

typedef struct  s_data {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
}               t_data;

typedef struct  s_game {
	void *mlx;
	void *win; 
	double cameraX;
	int w;
	int a;
	int s;
	int d;
	int left;
	int right;
	t_data data;
  t_conf conf;
}				t_game;

typedef enum  {
	W = 13,
	A = 0,
	S,
	D,
	LEFT  = 123,
	RIGHT,
} e_keys;

void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void		raycasting(t_game *game)
{
  int texWidth[4], texHeight[4]; 
  void *tex[4];
  tex[0] = mlx_xpm_file_to_image(game->mlx, game->conf.we_path, &texWidth[0], &texHeight[0]);
  tex[1] = mlx_xpm_file_to_image(game->mlx, game->conf.ea_path, &texWidth[1], &texHeight[1]);
  tex[2] = mlx_xpm_file_to_image(game->mlx, game->conf.so_path, &texWidth[2], &texHeight[2]);
  tex[3] = mlx_xpm_file_to_image(game->mlx, game->conf.no_path, &texWidth[3], &texHeight[3]);
  int bpp, sline, endian;
  int *texture[4];
  texture[0] = (int*)mlx_get_data_addr(tex[0], &bpp, &sline, &endian);
  texture[1] = (int*)mlx_get_data_addr(tex[1], &bpp, &sline, &endian);
  texture[2] = (int*)mlx_get_data_addr(tex[2], &bpp, &sline, &endian);
  texture[3] = (int*)mlx_get_data_addr(tex[3], &bpp, &sline, &endian);
	for(int x = 0; x < game->conf.width; x++)
    {
      //calculate ray position and direction
      double cameraX = 2 * x / (double)game->conf.width - 1; //x-coordinate in camera space
      double raydirX = game->conf.dirX + game->conf.planeX * cameraX;
      double rayDirY = game->conf.dirY + game->conf.planeY * cameraX;
      //which box of the map we're in
      int mapX = (int)game->conf.posX;
      int mapY = (int)game->conf.posY;

      //length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;

       //length of ray from one x or y-side to next x or y-side
      double deltaDistX = fabs(1 / raydirX);
      double deltaDistY = fabs(1 / rayDirY);
      double perpWallDist;

      //what direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; //was there a wall hit?
      int side; //was a NS or a EW wall hit?
      //calculate step and initial sideDist
      if(raydirX < 0)
      {
        stepX = -1;
        sideDistX = (game->conf.posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - game->conf.posX) * deltaDistX;
      }
      if(rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (game->conf.posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - game->conf.posY) * deltaDistY;
      }
      //perform DDA
      while (hit == 0)
      {
        //jump to next map square, OR in x-direction, OR in y-direction
        if(sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        //Check if ray has hit a wall
        if(game->conf.map[mapY][mapX] == '1') hit = 1;
      }
      //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
      if(side == 0) perpWallDist = (mapX - game->conf.posX + (1 - stepX) / 2) / raydirX;
      else          perpWallDist = (mapY - game->conf.posY + (1 - stepY) / 2) / rayDirY;

      //Calculate height of line to draw on screen
      int lineHeight = (int)(game->conf.height / perpWallDist);

      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + game->conf.height / 2;
      if(drawStart < 0)drawStart = 0;
      int drawEnd = lineHeight / 2 + game->conf.height / 2;
      if(drawEnd >= game->conf.height)drawEnd = game->conf.height - 1;
      //texturing calculations

      //calculate value of wallX
      double wallX; //where exactly the wall was hit
      if(side == 0) wallX = game->conf.posY + perpWallDist * rayDirY;
      else          wallX = game->conf.posX + perpWallDist * raydirX;
      wallX -= floor((wallX));

      //x coordinate on the texture
      int texX;
      if (side == 0) 
      {
        if (stepX < 0)
          texX = (int)(wallX * (double)(texWidth[0]));
        else
          texX = (int)(wallX * (double)(texWidth[1]));
      }
      else
      {
        if (stepY < 0)
          texX = (int)(wallX * (double)(texWidth[2]));
        else
          texX = (int)(wallX * (double)(texWidth[3]));
      }
      if(side == 0 && raydirX < 0) texX = texWidth[0] - texX - 1;
      else if(side == 0 && raydirX > 0) texX = texWidth[1] - texX - 1;
      else if(side == 1 && rayDirY < 0) texX = texWidth[2] - texX - 1;
      else if(side == 1 && rayDirY > 0) texX = texWidth[3] - texX - 1;

      // TODO: an integer-only bresenham or DDA like algorithm could make the texture coordinate stepping faster
      // How much to increase the texture coordinate per screen pixel
      double step;
      if (side == 0)
      {
        if (stepX < 0)
          step = 1.0 * texHeight[0] / lineHeight;
        else
          step = 1.0 * texHeight[1] / lineHeight;
      }
      else
      {
        if (stepY < 0)
          step = 1.0 * texHeight[2] / lineHeight;
        else
          step = 1.0 * texHeight[3] / lineHeight;
      }
	  for (int y = 0; y < drawStart; y++)
			my_mlx_pixel_put(&game->data, x, y, game->conf.ceil_col);
    double texPos = (drawStart - game->conf.height / 2 + lineHeight / 2) * step;
		for (int y = drawStart; y < drawEnd; y++)
		{
        int texY;
         
        texPos += step;
        int color;
        if (side == 0)
        {
          if (stepX < 0)
          {
            texY = (int)texPos & (texHeight[0] - 1);
            color = texture[0][texHeight[0] * texY + texX];
            my_mlx_pixel_put(&game->data, x, y, color);
          }
          else
          {
            texY = (int)texPos & (texHeight[1] - 1);
            color = texture[1][texHeight[1] * texY + texX];
            my_mlx_pixel_put(&game->data, x, y, color);
          }
        }
        else
        {
          if (stepY < 0)
          {
            texY = (int)texPos & (texHeight[2] - 1);
            color = texture[2][texHeight[2] * texY + texX];
            my_mlx_pixel_put(&game->data, x, y, color);
          }
          else
          {
            texY = (int)texPos & (texHeight[3] - 1);
            color = texture[3][texHeight[3] * texY + texX];
            my_mlx_pixel_put(&game->data, x, y, color);
          }
        }
		}
		for (int y = drawEnd; y < game->conf.height; y++)
			my_mlx_pixel_put(&game->data, x, y, game->conf.floor_col);
    }
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
	game->data.img = mlx_new_image(game->mlx, game->conf.width, game->conf.height);
	game->data.addr =  mlx_get_data_addr(game->data.img, &game->data.bits_per_pixel, &game->data.line_length, &game->data.endian);
	raycasting(game);
  //mlx_do_sync(game->mlx);
	mlx_put_image_to_window(game->mlx, game->win, game->data.img, 0, 0);
	mlx_destroy_image(game->mlx, game->data.img);
	return (0);
}

int key_press(int key, t_game *game)
{
	printf("%d\n", key);
	if (key == W)
		game->w = 1;
	if (key == S)
		game->s = 1;
	if (key == LEFT)
		game->left = 1;
	if (key == RIGHT)
		game->right = 1;
  if (key == A)
		game->a = 1;
  if (key == D)
		game->d = 1;
	printf("%d\n", game->right);
	return (0);
}

int key_unpress(int key, t_game *game)
{
	if (key == W)
		game->w = 0;
	if (key == S)
		game->s = 0;
	if (key == LEFT)
		game->left = 0;
	if (key == RIGHT)
		game->right = 0;
  if (key == A)
		game->a = 0;
  if (key == D)
		game->d = 0;
	printf("%d\n", game->right);
	return (0);
}

int main(int argc, char **argv)
{
  t_game game;
	game.w = 0;
	game.s = 0;
	game.a = 0;
	game.d = 0;
  game.left = 0;
  game.right = 0;
  if (argc > 2)
    return (0);
	ft_parsing(argv[1], &game.conf);
	game.mlx = mlx_init();
	game.win = mlx_new_window(game.mlx, game.conf.width, game.conf.height, "Hello!");
  check_state(&game);
	game.data.img = mlx_new_image(game.mlx, game.conf.width, game.conf.height);
	game.data.addr =  mlx_get_data_addr(game.data.img, &game.data.bits_per_pixel, &game.data.line_length, &game.data.endian);
	raycasting(&game);
  //mlx_do_sync(game->mlx);
	mlx_put_image_to_window(game.mlx, game.win, game.data.img, 0, 0);
	mlx_destroy_image(game.mlx, game.data.img);
	mlx_loop_hook(game.mlx, draw_frame, &game);
	mlx_hook(game.win, 2, (1L<<0), key_press, &game);
	mlx_hook(game.win, 3, (1L<<1), key_unpress, &game);
	mlx_loop(game.mlx);
}