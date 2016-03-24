/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   callback.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/24 08:18:18 by nbouteme          #+#    #+#             */
/*   Updated: 2016/03/24 08:18:57 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "callback.h"

void	print_list_help(t_list *elem, t_options *opts)
{
	print_gen_file(elem->content, opts);
}

int		keep_things(t_list *elem, t_options *opts)
{
	char *name;

	name = elem->content;
	return (name[0] != '.' || opts->flags['a']);
}

void	error_handle(t_fileinfo *elem, t_options *opts)
{
	extern int g_errored;

	(void)opts;
	ft_putstr("ls: ");
	g_errored = 1;
	errno = elem->e;
	perror(elem->name);
}

void	disp(t_list *list, t_options *opts)
{
	ft_lstsortup(&list, (void*)&cmp_args, opts);
	ft_lstiterup(list, (void *)&render_list, opts);
}

void	print_gen_file(t_fileinfo *elem, t_options *opts)
{
	if (opts->flags['l'])
		return (print_long(elem, opts));
	if (S_ISLNK(elem->info.st_mode) && S_ISDIR(elem->real_info->st_mode))
		return (render_dir(elem, opts));
	ft_putendl(elem->name);
}
