/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 16:29:11 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/15 16:02:33 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "libft.h"
#include "cub3d.h"

char		*check_path(t_conf *conf, char *line)
{
	char	*s;

	chk_tag(conf, line, ".xpm");
	s = check_strdup(line, conf);
	return (s);
}

void		ft_flags(char **spl_line, char *line, t_list *lst, t_conf *conf)
{
	if (spl_line[0] == NULL)
		return ;
	else if (!(ft_strncmp("R", spl_line[0], 2)) && !(spl_line[3]))
		frame_res(spl_line, conf);
	else if (!(ft_strncmp("NO", spl_line[0], 3)) && !(spl_line[2]))
		conf->no_path = check_path(conf, spl_line[1]);
	else if (!(ft_strncmp("SO", spl_line[0], 3)) && !(spl_line[2]))
		conf->so_path = check_path(conf, spl_line[1]);
	else if (!(ft_strncmp("WE", spl_line[0], 3)) && !(spl_line[2]))
		conf->we_path = check_path(conf, spl_line[1]);
	else if (!(ft_strncmp("EA", spl_line[0], 3)) && !(spl_line[2]))
		conf->ea_path = check_path(conf, spl_line[1]);
	else if (!(ft_strncmp("S", spl_line[0], 2)) && !(spl_line[2]))
		conf->s_path = check_path(conf, spl_line[1]);
	else if (!(ft_strncmp("F", spl_line[0], 2)) && !(spl_line[2]))
		conf->floor_col = ft_colors(spl_line[1], conf);
	else if (!(ft_strncmp("C", spl_line[0], 2)) && !(spl_line[2]))
		conf->ceil_col = ft_colors(spl_line[1], conf);
	else
		list_add(conf, lst, line);
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

	lst = ft_lstnew(0);
	if ((fd = open(file, O_RDONLY)) < 0)
		ft_error(conf, lst, 0, 0);
	while ((get_next_line(fd, &line)) > 0)
		read_line(&spl_line, &line, conf, lst);
	read_line(&spl_line, &line, conf, lst);
	ft_map(lst, conf, line, spl_line);
}
