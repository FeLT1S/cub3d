/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <jiandre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:29:11 by jiandre           #+#    #+#             */
/*   Updated: 2020/10/06 15:37:26 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "libft.h"
#include "cub3d.h"

void		check_path(t_conf *conf, char **line, char **s)
{
	if (line[2])
		conf->err = 3;
	if (line[1])
		chk_tag(conf, line[1], ".xpm");
	if (conf->ymp == 1)
		conf->err = 6;
	if (!*s && line[1])
		*s = check_strdup(line[1], conf);
	else
		conf->err = -1;
}

void		chk_empty(t_conf *conf, t_list *lst)
{
	char	*str;
	int		i;

	str = ft_lstlast(lst)->content;
	i = 0;
	if (conf->ymp == 1)
	{
		while (str[i])
		{
			if (!(ft_memchr("1 ", str[i], 3)) && !(ft_strncmp("1", str, 2)))
				conf->err = 4;
			i++;
		}
		list_add(conf, lst, "  ");
	}
}

void		ft_flags(char **spl_line, char *line, t_list *lst, t_conf *conf)
{
	if (spl_line[0] == NULL)
		chk_empty(conf, lst);
	else if (!(ft_strncmp("R", spl_line[0], 2)))
		frame_res(spl_line, conf);
	else if (!(ft_strncmp("NO", spl_line[0], 3)))
		check_path(conf, spl_line, &conf->no_path);
	else if (!(ft_strncmp("SO", spl_line[0], 3)))
		check_path(conf, spl_line, &conf->so_path);
	else if (!(ft_strncmp("WE", spl_line[0], 3)))
		check_path(conf, spl_line, &conf->we_path);
	else if (!(ft_strncmp("EA", spl_line[0], 3)))
		check_path(conf, spl_line, &conf->ea_path);
	else if (!(ft_strncmp("S", spl_line[0], 2)))
		check_path(conf, spl_line, &conf->s_path);
	else if (!(ft_strncmp("F", spl_line[0], 2)))
		ft_colors(spl_line, conf, &conf->floor_col);
	else if (!(ft_strncmp("C", spl_line[0], 2)))
		ft_colors(spl_line, conf, &conf->ceil_col);
	else if ((!(ft_strncmp("0", spl_line[0], 1)) ||
	!(ft_strncmp("1", spl_line[0], 1))) && (conf->ymp = 1))
		list_add(conf, lst, line);
	else
		conf->err = 9;
}

void		read_line(char ***spl_line, char **line, t_conf *conf, t_list *lst)
{
	if (!(*spl_line = ft_split(*line, ' ')))
		ft_error(conf, lst, *line, *spl_line);
	ft_flags(*spl_line, *line, lst, conf);
	free(*line);
	*line = 0;
	splitfree(*spl_line);
	*spl_line = 0;
}

void		ft_parsing(char *file, t_conf *conf)
{
	t_list	*lst;
	int		fd;
	char	*line;
	char	**spl_line;

	if (conf->err)
		nval(conf, conf->err);
	lst = ft_lstnew(0);
	if ((fd = open(file, O_RDONLY)) < 0)
		nval(conf, 7);
	while ((get_next_line(fd, &line)) > 0)
		read_line(&spl_line, &line, conf, lst);
	read_line(&spl_line, &line, conf, lst);
	ft_map(lst, conf, line, spl_line);
}
