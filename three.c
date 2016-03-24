/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   three.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/24 09:28:52 by nbouteme          #+#    #+#             */
/*   Updated: 2016/03/24 09:29:03 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

t_field	*get_field(t_fileinfo *elem, const char *key)
{
	t_list *f;

	f = elem->fields;
	while (f)
	{
		if (ft_strcmp(((t_field *)f->content)->key, key) == 0)
			return ((t_field *)f->content);
		f = f->next;
	}
	return (0);
}

void	bake_fields(t_list *elem, t_options *opts)
{
	t_fileinfo	*file;
	char		*s;

	file = elem->content;
	add_field(file, "name", render_fn(file, opts));
	if (!opts->flags['l'])
		return ;
	add_field(file, "perms", render_perms(file));
	add_field(file, "hl", ft_itoa(file->info.st_nlink));
	add_field(file, "user", ft_strjoin(user_from_id(file->info.st_uid), " "));
	add_field(file, "group", group_from_id(file->info.st_gid));
	if (S_ISCHR(file->info.st_mode) || S_ISBLK(file->info.st_mode))
	{
		s = ft_strcat(ft_itoa(major(file->info.st_rdev)), ",");
		ft_memmove(s + 1, s, ft_strlen(s) + 1);
		s[0] = ' ';
		add_field(file, "major", s);
		add_field(file, "size", s = ft_itoa(minor(file->info.st_rdev)));
	}
	else
		add_field(file, "size", s = ft_itoa(file->info.st_size));
	s = my_ctime(&file->info.st_mtim.tv_sec);
	add_field(file, "ctime", s);
}

void	calc_pads(t_list *elem, t_padargs *args)
{
	t_list	*l;
	t_field	*c;
	int		w;

	l = ((t_fileinfo *)elem->content)->fields;
	while (l)
	{
		c = get_field(args->pads, ((t_field *)l->content)->key);
		w = ((t_field *)l->content)->width;
		c->width = c->width < w ? w : c->width;
		l = l->next;
	}
}

void	padded_print(t_list *elem, t_padargs *args)
{
	int		i;
	int		n;
	t_field	*f;
	t_field	*g;

	i = -1;
	while (++i < 8)
	{
		f = get_field(((t_fileinfo *)elem->content), g_field[i]);
		n = f ? f->width : 0;
		g = get_field(args->pads, g_field[i]);
		if (!g || !f)
			continue;
		n = (g ? g->width : 0) - n;
		while (g_pad[i] && n-- > 0)
			ft_putchar(' ');
		ft_putstr(f ? f->value : "");
		if (ft_strcmp("name", g_field[i]) == 0)
			return (ft_putstr("\n"));
		while (!g_pad[i] && n-- > 0)
			ft_putchar(' ');
		ft_putchar(' ');
	}
}

void	simple_print(t_list *elem, t_padargs *args)
{
	(void)args;
	ft_putstr(get_field(((t_fileinfo *)elem->content), "name")->value);
	ft_putstr("\n");
}
