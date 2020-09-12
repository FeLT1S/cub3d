/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:46:09 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/12 19:21:31 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx/mlx.h"
#include "keys.h"
#include "libft.h"
#include <stdio.h>

typedef struct		s_sprite
{
	double			x;
	double			y;
}               	t_sprite;

typedef struct		s_conf
{
	void			*mlx;
	int				height;
	int				width;
	char			*no_path;
	char			*so_path;
	char			*we_path;
	char			*ea_path;
	char			*s_path;
	int				floor_col;
	int				ceil_col;
	char			**map;
	double 			posX; 
	double			posY;
	double			dirX; 
	double			dirY;
	double			planeX;
	double			planeY;
	int				num_sprs;
	int				num_spawns;
	char			err;
	t_sprite 		*sprite;
}					t_conf;

typedef struct  	s_data {
    void        	*img;
    char        	*addr;
    int         	bits_per_pixel;
    int         	line_length;
    int         	endian;
}               	t_data;

typedef struct  	s_game {
	void			*win; 
	double			cameraX;
	int				w;
	int				a;
	int				s;
	int				d;
	int				left;
	int				right;
	t_data			data;
	t_conf			conf;
}					t_game;

void		ft_parsing(char *file, t_conf *conf);