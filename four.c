/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   four.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/24 09:29:20 by nbouteme          #+#    #+#             */
/*   Updated: 2016/03/24 09:29:30 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	delete_field(void *elem, size_t size)
{
	t_field *f;

	(void)size;
	f = elem;
	free(f->key);
	free(f);
}

void	fileinfo_dtor(t_fileinfo *s)
{
	if (s->fields)
		ft_lstdel(&s->fields, &delete_field);
	if (s->name)
		free(s->name);
	if (s->real_info)
		free(s->real_info);
	free(s);
}

int		one_of_is_blk(t_list *files)
{
	t_list		*l;
	t_fileinfo	*fi;

	l = files;
	while (l)
	{
		fi = l->content;
		if (fi->info.st_mode & S_IFCHR)
			return (1);
		l = l->next;
	}
	return (0);
}

void	print_dir(t_list *content, t_options *opts)
{
	t_fileinfo	*f;
	t_padargs	args;

	f = ft_memalloc(sizeof(*f));
	add_field(f, "name", "");
	if (opts->flags['l'])
	{
		add_field(f, "perms", "");
		add_field(f, "hl", "");
		add_field(f, "user", "");
		add_field(f, "group", "");
		if (one_of_is_blk(content))
			add_field(f, "major", "");
		add_field(f, "size", "");
		add_field(f, "ctime", "");
	}
	args = (t_padargs){f, opts};
	ft_lstiterup(content, (void*)&calc_pads, &args);
	if (opts->flags['l'])
		get_field(f, "size")->width++;
	if (opts->flags['l'])
		ft_lstiterup(content, (void*)&padded_print, &args);
	else
		ft_lstiterup(content, (void*)&simple_print, &args);
	fileinfo_dtor(f);
}

int		keep_dirs(t_list *elem)
{
	t_fileinfo *f;

	f = elem->content;
	if (ft_strcmp(f->name, ".") == 0 || ft_strcmp(f->name, "..") == 0)
		return (0);
	return (S_ISDIR(f->info.st_mode));
}
