/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 18:20:30 by jiandre           #+#    #+#             */
/*   Updated: 2020/08/31 20:05:07 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <math.h>
#include <stdio.h>

#define screenWidth 640
#define screenHeight 480
#define mapWidth 24
#define mapHeight 24
#define h 480
#define moveSpeed 0.1
#define rotSpeed 0.05

int worldMap[mapHeight][mapWidth]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

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
	double posX; 
	double posY;
	double dirX; 
	double dirY;
	double cameraX;
	
	double planeX;
	double planeY;
	int w;
	int a;
	int s;
	int d;
	int left;
	int right;
	t_data data;
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
	for(int x = 0; x < screenWidth; x++)
    {
      //calculate ray position and direction
      double cameraX = 2 * x / (double)screenWidth - 1; //x-coordinate in camera space
      double rayDirX = game->dirX + game->planeX * cameraX;
      double rayDirY = game->dirY + game->planeY * cameraX;
      //which box of the map we're in
      int mapX = (int)game->posX;
      int mapY = (int)game->posY;

      //length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;

       //length of ray from one x or y-side to next x or y-side
      double deltaDistX = fabs(1 / rayDirX);
      double deltaDistY = fabs(1 / rayDirY);
      double perpWallDist;

      //what direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; //was there a wall hit?
      int side; //was a NS or a EW wall hit?
      //calculate step and initial sideDist
      if(rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (game->posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - game->posX) * deltaDistX;
      }
      if(rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (game->posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - game->posY) * deltaDistY;
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
        if(worldMap[mapX][mapY] > 0) hit = 1;
      }
      //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
      if(side == 0) perpWallDist = (mapX - game->posX + (1 - stepX) / 2) / rayDirX;
      else          perpWallDist = (mapY - game->posY + (1 - stepY) / 2) / rayDirY;

      //Calculate height of line to draw on screen
      int lineHeight = (int)(h / perpWallDist);

      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + h / 2;
      if(drawStart < 0)drawStart = 0;
      int drawEnd = lineHeight / 2 + h / 2;
      if(drawEnd >= h)drawEnd = h - 1;
	  	for (int y = 0; y < drawStart; y++)
			my_mlx_pixel_put(&game->data, x, y, 0x000000ff);
		for (int y = drawStart; y < drawEnd; y++)
		{
			if (!side)
				my_mlx_pixel_put(&game->data, x, y, 0x0000ff00);
			else
				my_mlx_pixel_put(&game->data, x, y, 0x0000ff00 / 2);
		}
		for (int y = drawEnd; y < screenHeight; y++)
			my_mlx_pixel_put(&game->data, x, y, 0x00f0ff00);
    }
}

void		check_state(t_game *game)
{
	if (game->w)
	{
		if(worldMap[(int)(game->posX + game->dirX * moveSpeed)][(int)(game->posY)] == 0) game->posX += game->dirX * moveSpeed;
		if(worldMap[(int)(game->posX)][(int)(game->posY + game->dirY * moveSpeed)] == 0) game->posY += game->dirY * moveSpeed;
	}
	if (game->s)
	{
		if(worldMap[(int)(game->posX - game->dirX * moveSpeed)][(int)game->posY] == 0) game->posX -= game->dirX * moveSpeed;
		if(worldMap[(int)(game->posX)][(int)(game->posY - game->dirY * moveSpeed)] == 0) game->posY -= game->dirY * moveSpeed;
	}
	if (game->left)
	{
      double oldDirX = game->dirX;
      game->dirX = game->dirX * cos(rotSpeed) - game->dirY * sin(rotSpeed);
      game->dirY = oldDirX * sin(rotSpeed) + game->dirY * cos(rotSpeed);
      double oldPlaneX = game->planeX;
      game->planeX = game->planeX * cos(rotSpeed) - game->planeY * sin(rotSpeed);
      game->planeY = oldPlaneX * sin(rotSpeed) + game->planeY * cos(rotSpeed);
	}
	if (game->right)
	{
      double oldDirX = game->dirX;
      game->dirX = game->dirX * cos(-rotSpeed) - game->dirY * sin(-rotSpeed);
      game->dirY = oldDirX * sin(-rotSpeed) + game->dirY * cos(-rotSpeed);
      double oldPlaneX = game->planeX;
      game->planeX = game->planeX * cos(-rotSpeed) - game->planeY * sin(-rotSpeed);
      game->planeY = oldPlaneX * sin(-rotSpeed) + game->planeY * cos(-rotSpeed);
	}
}

int			draw_frame(t_game *game)
{
	if (!(game->w || game->s || game->left || game->right))
		return(0);
	check_state(game);
	game->data.img = mlx_new_image(game->mlx, 640, 480);
	game->data.addr =  mlx_get_data_addr(game->data.img, &game->data.bits_per_pixel, &game->data.line_length, &game->data.endian);
	raycasting(game);
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
	printf("%d\n", game->w);
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
	printf("%d\n", game->w);
	return (0);
}

int main()
{

  //(screenWidth, screenHeight, 0, "Raycaster");
  	t_game game;
	game.w = 0;
	game.s = 0;
	game.posX = 22;
	game.posY = 12;  //x and y start position
	game.dirX = -1;
	game.dirY = 0; //initial direction vector
	game.planeX = 0;
	game.planeY = 0.66; //the 2d raycaster version of camera plane
	game.mlx = mlx_init();
	game.win = mlx_new_window(game.mlx, 640, 480, "Hello!");
	game.data.img = mlx_new_image(game.mlx, 640, 480);
	game.data.addr = mlx_get_data_addr(game.data.img, &game.data.bits_per_pixel, &game.data.line_length, &game.data.endian);
	
	raycasting(&game);
	mlx_put_image_to_window(game.mlx, game.win, game.data.img, 0, 0);
	mlx_destroy_image(game.mlx, game.data.img);
	mlx_loop_hook(game.mlx, draw_frame, &game);
	mlx_hook(game.win, 2, (1L<<0), key_press, &game);
	mlx_hook(game.win, 3, (1L<<1), key_unpress, &game);
	mlx_loop(game.mlx);
    // //move forward if no wall in front of you
    // if(keyDown(SDLK_UP))
    // {
    //   if(worldMap[int(posX + dirX * moveSpeed)][int(posY)] == 0) posX += dirX * moveSpeed;
    //   if(worldMap[int(posX)][int(posY + dirY * moveSpeed)] == 0) posY += dirY * moveSpeed;
    // }
    // //move backwards if no wall behind you
    // if(keyDown(SDLK_DOWN))
    // {
    //   if(worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
    //   if(worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
    // }
    // //rotate to the right
    // if(keyDown(SDLK_RIGHT))
    // {
    //   //both camera direction and camera plane must be rotated
    //   double oldDirX = dirX;
    //   dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
    //   dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
    //   double oldPlaneX = planeX;
    //   planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
    //   planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    // }
    // //rotate to the left
    // if(keyDown(SDLK_LEFT))
    // {
    //   //both camera direction and camera plane must be rotated
    //   double oldDirX = dirX;
    //   dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
    //   dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
    //   double oldPlaneX = planeX;
    //   planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
    //   planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    // }
}