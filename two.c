/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   two.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/24 09:28:27 by nbouteme          #+#    #+#             */
/*   Updated: 2016/03/24 09:37:54 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	print_fn(t_fileinfo *elem, t_options *opts)
{
	char *tmp;
	char *ft;

	ft_putstr(elem->name);
	if (S_ISLNK(elem->info.st_mode))
	{
		tmp = ft_strnew(512);
		if (opts->flags['l'])
			ft_putstr(" -> ");
		ft = elem->name[0] != '/' ? ft_strjoin(set_cwdir(0), elem->name)
			: ft_strdup(elem->name);
		readlink(ft, tmp, 1024);
		ft_putstr(tmp);
		free(ft);
		free(tmp);
	}
	if (opts->flags['l'])
		ft_putstr("\n");
	else
		ft_putchar('\t');
}

char	*my_ctime(const time_t *clock)
{
	char	*s;
	time_t	cur;

	cur = time(0);
	s = ctime(clock);
	s = ft_strdup(s);
	if (ABS(*clock - cur) > 15778463)
	{
		ft_memcpy(s + 12, s + 20, 4);
		s[11] = 32;
	}
	s = ft_strsub(s, 4, 12);
	return (s);
}

void	print_long(t_fileinfo *elem, t_options *opts)
{
	char *s;

	ft_putstr(render_perms(elem));
	ft_putchar(32);
	ft_putnbr(elem->info.st_nlink);
	ft_putchar(32);
	ft_putstr(user_from_id(elem->info.st_uid));
	ft_putchar(32);
	ft_putchar(32);
	ft_putstr(group_from_id(elem->info.st_gid));
	ft_putchar(32);
	ft_putchar(32);
	ft_putnbr(elem->info.st_size);
	ft_putchar(32);
	s = my_ctime(&elem->info.st_mtimespec.tv_sec);
	write(1, s, ft_strlen(s));
	ft_putchar(32);
	print_fn(elem, opts);
}

char	*render_fn(t_fileinfo *elem, t_options *opts)
{
	char *t;
	char *r;
	char *tmp;
	char *ft;

	r = ft_strdup(elem->name);
	if (S_ISLNK(elem->info.st_mode) && opts->flags['l'])
	{
		t = ft_strjoin(r, " -> ");
		tmp = ft_strnew(512);
		ft = ft_strjoin(set_cwdir(0), elem->name);
		readlink(ft, tmp, 1024);
		r = ft_strjoin(t, tmp);
		free(t);
		free(ft);
		free(tmp);
	}
	return (r);
}

void	add_field(t_fileinfo *elem, const char *key, char *value)
{
	t_field f;

	f.key = ft_strdup(key);
	f.value = value;
	f.width = ft_strlen(value);
	ft_lstadd(&elem->fields, ft_lstnew(&f, sizeof(f)));
}
