/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:46:09 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/10 19:56:10 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx/mlx.h"
#include "keys.h"
#include "libft.h"
#include <stdio.h>

//"R ", "NO ", "SO ", "WE ", "EA ", "S ", "F ", "C "

typedef struct s_sprite
{
  double x;
  double y;
}               t_sprite;

typedef struct		s_conf
{
	void	*mlx;
	int		height;
	int		width;
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	char	*s_path;
	int		floor_col;
	int		ceil_col;
	char		**map;
	double 	posX; 
	double	posY;
	double	dirX; 
	double	dirY;
	double planeX;
	double planeY;
	int		numSprites;
	int		num_spawns;
	t_sprite *sprite;
}					t_conf;

void		ft_parsing(char *file, t_conf *conf);