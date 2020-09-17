/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:46:09 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/17 20:10:28 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mlx/mlx.h"
#include "keys.h"
#include "libft.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define mvspd 0.06
#define rtspd 0.04

typedef struct		s_sprite
{
	double			x;
	double			y;
}               	t_sprite;

typedef struct  	s_tex {
	void			*img;
	int				*addr;
	int				wdth;
	int				hght;
	int         	bpp;
    int         	l_ln;
    int         	enan;
	int				x;
	int				y;
}					t_tex;

typedef struct  	s_data {
    void        	*img;
    char        	*addr;
    int         	bpp;
    int         	l_ln;
    int         	enan;
}               	t_data;

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
	double 			posx; 
	double			posy;
	double			dirx; 
	double			diry;
	double			plx;
	double			ply;
	int				num_sprs;
	int				num_spawns;
	char			err;
	t_sprite 		*sprite;
	t_data			data;
	t_tex			tex[5];
}					t_conf;

typedef struct  	s_game {
	void			*win; 
	double			cameraX;
	int				w;
	int				a;
	int				s;
	int				d;
	int				left;
	int				right;
	t_conf			conf;
}					t_game;

typedef struct		s_rc{
	double			camerax;
	double			raydirx;
	double			raydiry;
	int				mapx;
	int				mapy;
	double			sidedistx;
    double			sidedisty;
	double			ddistx;
    double			ddisty;
    double			prpwldist;
    int				stepx;
    int				stepy;
    int				hit;
    int				side;
	int				linehgt;
	int				drwstrt;
	int				drwend;
	double			wlx;
	int				texx;
	double			step;
	double			tx_pos;
	int				x;
	int				y;
}					t_rc;

typedef struct  	s_sc {
	double			x;
	double			y;
	double			invdet;
	double			transx;
	double			transy;
	int				screenx;
	int				hght;
	int				wdth;
	int				draw_starty;
	int				draw_endy;
	int				draw_startx;
	int				draw_endx;
	int				texx;
	int				texy;
}					t_sc;

void		ft_parsing(char *file, t_conf *conf);
void		splitfree(char **out);
void		ft_error(t_conf *conf, t_list *lst, char *line, char **spl_line);
char		*check_strdup(const char *s, t_conf *conf);
int			ft_colors(char *col_line, t_conf *conf);
void		list_add(t_conf *conf, t_list *lst, char *line);
void		frame_res(char **spl_line, t_conf *conf);
void		check_pos(t_conf *conf, int i, int j);
void		ft_map(t_list *lst, t_conf *conf, char *line, char **spl_line);
void		chk_tag(t_conf *conf, char *s, char *tag);
void		raycasting(t_conf *conf);
void		rc_texx(t_rc *rc, t_conf *conf, t_tex *tex);
void		rc_texstep(t_rc *rc, t_tex *tex);
void		rc_paint_wall(t_rc *rc, t_conf *conf, t_tex tex);
void		rc_walls(t_rc *rc, t_conf *conf, t_tex *tx);
void		tex_init(t_tex *tex, t_conf *conf);
void		spritecast(t_conf *conf, double *zbuff, t_tex tex);
void        my_mlx_pixel_put(t_data *data, int x, int y, int color);
void    	sort(int *sprite_order, double *spriteDistance, int amount);
void		check_state(t_game *game);
int			ft_close(t_conf *conf);
int			save_bmp(t_game *game);