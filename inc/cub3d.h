/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:46:09 by jiandre           #+#    #+#             */
/*   Updated: 2020/08/28 19:58:21 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"

//"R ", "NO ", "SO ", "WE ", "EA ", "S ", "F ", "C "

typedef struct		s_conf
{
	int		height;
	int		width;
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	char	*s_path;
	int		floor_col;
	int		ceil_col;
}					t_conf;