/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiandre <kostbg1@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 18:34:30 by jiandre           #+#    #+#             */
/*   Updated: 2020/09/21 18:41:26 by jiandre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		splitfree(char **out)
{
	size_t	i;

	i = 0;
	while (out[i])
	{
		if (out[i])
			free(out[i]);
		i++;
	}
	free(out);
}

void		ft_error(t_conf *conf, t_list *lst, char *line, char **spl_line)
{
	write(2, "Error\n", 7);
	code_error(conf);
	if (conf->no_path)
		free(conf->no_path);
	if (conf->so_path)
		free(conf->so_path);
	if (conf->we_path)
		free(conf->we_path);
	if (conf->ea_path)
		free(conf->ea_path);
	if (conf->s_path)
		free(conf->s_path);
	if (conf->map)
		splitfree(conf->map);
	if (conf->sprite)
		free(conf->sprite);
	if (lst)
		ft_lstclear(&lst, free);
	if (line)
		free(line);
	if (spl_line)
		splitfree(spl_line);
	exit(0);
}

char		*check_strdup(const char *s, t_conf *conf)
{
	char	*new_str;

	if (!(new_str = ft_strdup(s)))
		conf->err = -1;
	return (new_str);
}

void		list_add(t_conf *conf, t_list *lst, char *line)
{
	char	*spr_line;
	char	*new_line;

	new_line = check_strdup(line, conf);
	ft_lstadd_back(&lst, ft_lstnew(new_line));
	spr_line = new_line;
	while ((spr_line = ft_strchr(spr_line++, '2')) && spr_line++)
		conf->num_sprs++;
}

void		chk_tag(t_conf *conf, char *s, char *tag)
{
	s = ft_strrchr(s, tag[0]);
	if (!s || (ft_strncmp(s, tag, ft_strlen(s))))
		conf->err = 6;
}
