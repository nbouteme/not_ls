/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 12:39:13 by nbouteme          #+#    #+#             */
/*   Updated: 2016/03/24 09:31:44 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

int			g_first = 1;
int			g_errored = 0;
const char	*g_file_type = "-pc-d-b---l-s";
const char	*g_field[] =
{
	"perms", "hl", "user", "group", "major", "size", "ctime", "name"
};
const int	g_pad[] = { 0, 1, 0, 0, 1, 1, 1, 0 };

int		should_print_name(t_fileinfo *elem, t_options *opts)
{
	t_list *l;

	if (opts->argc > 1)
		return (1);
	l = opts->files;
	while (l)
	{
		if (strcmp(elem->name, l->content) == 0)
			return (0);
		l = l->next;
	}
	return (1);
}

void	dir_cont(t_list *dir_c, t_options *opts)
{
	t_list	*rec_content;

	if (!dir_c)
		return ;
	if (opts->flags['l'])
	{
		ft_putstr("total ");
		ft_putnbr(get_total_blocks(dir_c));
		ft_putstr("\n");
	}
	ft_lstiterup(dir_c, (void*)&bake_fields, opts);
	print_dir(dir_c, opts);
	if (opts->flags['R'] && (rec_content = ft_lstfilter(dir_c, &keep_dirs)))
		disp(rec_content, opts);
	ft_lstdel(&dir_c, &delete_fileinfo);
}

void	render_dir(t_fileinfo *elem, t_options *opts)
{
	t_list	*dir_content;
	char	*tmp;
	char	*cat;

	tmp = ft_strdup(set_cwdir(0));
	cat = ft_strjoin(set_cwdir(0), elem->name);
	set_cwdir(elem->name[0] == '/' ? elem->name : cat);
	if (should_print_name(elem, opts))
	{
		ft_putstr(g_first ? "" : "\n");
		ft_putstr(ft_strncmp(cat, "./", 2) == 0 ? cat + 2 : elem->name);
		ft_putstr(":\n");
	}
	g_first &= 0;
	free(cat);
	dir_content = get_content(set_cwdir(0), opts);
	dir_cont(dir_content, opts);
	set_cwdir(tmp);
	free(tmp);
}

void	render_list(t_list *elem, t_options *opts)
{
	t_fileinfo		*obj;
	const t_renderf	rndr_type[] = { &error_handle, &print_gen_file,
									&print_gen_file, 0, &render_dir, 0,
									&print_gen_file, 0, &print_gen_file, 0,
									&print_gen_file, 0, &print_gen_file };

	obj = elem->content;
	rndr_type[(obj->info.st_mode & S_IFMT) >> 12](obj, opts);
	if (g_first && !g_errored)
		g_first = 0;
	else
		g_errored = 0;
}
