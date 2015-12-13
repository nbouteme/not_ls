/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 12:39:13 by nbouteme          #+#    #+#             */
/*   Updated: 2015/12/13 11:58:22 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

char *perms2str(mode_t mode)
{
	const char *file_type = "-pc-d-b---l-s";
	char *str;
	char *o;
	int i;
	int s;
	int n;

	str = calloc(1, 11);
	o = str;
	*str++ = file_type[(mode & S_IFMT) >> 12];
	s = mode & 01000;
	mode &= 0777;
	i = 3;
	while (i--)
	{
		n = (mode & (07 << (3 * i))) >> (3 * i);
		*str++ = n & 4 ? 'r' : '-';
		*str++ = n & 2 ? 'w' : '-';
		if(i == 0 && s)
			*str++ = n & 1 ? 't' : 'T';
		else
			*str++ = n & 1 ? 'x' : '-';
	}
	return o;
}

#include <sys/xattr.h>

int has_extended(t_fileinfo *f)
{
	int n;
	char *tmp;

	tmp = ft_strjoin(set_cwdir(0), f->name);
	n = listxattr(tmp, 0, 0, XATTR_NOFOLLOW) > 0;
	free(tmp);
	return n;
}

#include <sys/types.h>
#include <sys/acl.h>
#include <unistd.h>
#include <fcntl.h>


int has_acl(t_fileinfo *f)
{
	char *tmp;
	int fd;

	tmp = ft_strjoin(set_cwdir(0), f->name);
	fd = open(tmp, O_RDONLY);
	acl_t acl = acl_get_fd(fd);
	acl_free(acl);
	free(tmp);
	return !!acl;
}

char *render_perms(t_fileinfo *f)
{
	char *s;
	char *ret;

	s = perms2str(f->info.st_mode);
	ret = ft_strjoin(s, " ");
	ret[10] = has_acl(f) ? '+' : ' ';
	ret[10] = has_extended(f) ? '@' : ret[10];
	free(s);
	return ret;
}

void print_mode_str(t_fileinfo *f)
{
	char *s;

	s = perms2str(f->info.st_mode);
	ft_putstr(s);
	free(s);
}

void print_fn(t_fileinfo *elem, t_options *opts)
{
	// couleurs ici
	ft_putstr(elem->name);
	// couleurs ici
	if(opts->long_format)
		ft_putchar(10);
	else
		ft_putchar('\t');
}

void print_long(t_fileinfo *elem, t_options *opts)
{
	(void)opts;
	print_mode_str(elem);
	ft_putchar(32);
	ft_putnbr(elem->info.st_nlink);
	ft_putchar(32);
	ft_putstr(user_from_id(elem->info.st_uid));
	ft_putchar(32);
	ft_putstr(group_from_id(elem->info.st_gid));
	ft_putchar(32);
	ft_putnbr(elem->info.st_size);
	ft_putchar(32);
	char *s = ctime(&elem->info.st_mtim.tv_sec);
	write(1, s, ft_strlen(s) - 1);
	ft_putchar(32);
	print_fn(elem, opts);
}

char *render_fn(t_fileinfo *elem, t_options *opts)
{
	(void)opts;//add colors
	return elem->name;
}

typedef struct	s_field
{
	char	*key;
	char	*value;
	int		width;
}				t_field;

void add_field(t_fileinfo *elem, const char *key, const char *value)
{
	t_field f;

	f.key = ft_strdup(key);
	f.value = ft_strdup(value);
	f.width = ft_strlen(value);
	ft_lstadd(&elem->fields, ft_lstnew(&f, sizeof(f)));
}

t_field *get_field(t_fileinfo *elem, const char *key)
{
	t_list *f;

	f = elem->fields;
	while (f)
	{
		if(ft_strcmp(((t_field *)f->content)->key, key) == 0)
			return ((t_field *)f->content);
		f = f->next;
	}
	return 0;
}

void bake_fields(t_list *elem, t_options *opts)
{
	t_fileinfo *file;
	char *s;
	file = elem->content;
	add_field(file, "name", render_fn(file, opts));
	if(!opts->long_format)
		return;
	add_field(file, "perms", render_perms(file));
	add_field(file, "hl", ft_itoa(file->info.st_nlink));
	add_field(file, "user", ft_strjoin(user_from_id(file->info.st_uid), " "));
	add_field(file, "group", ft_strjoin(group_from_id(file->info.st_gid), " "));
	add_field(file, "size", ft_itoa(file->info.st_size));
	s = ctime(&file->info.st_mtim.tv_sec);
	s = ft_strsub(s, 4, ft_strlen(s) - 13);
	add_field(file, "ctime", s);
}

typedef struct	s_padargs
{
	t_fileinfo *pads;
	t_options *opts;
}				t_padargs;

void calc_pads(t_list *elem, t_padargs *args)
{
	t_list *l;
	t_field *c;
	int w;

	l = ((t_fileinfo *)elem->content)->fields;
	while(l)
	{
		c = get_field(args->pads, ((t_field *)l->content)->key);
		w = ((t_field *)l->content)->width;
		c->width = c->width < w ? w : c->width;
		l = l->next;
	}
}

void padded_print(t_list *elem, t_padargs *args)
{
	const char *field[] = { "perms", "hl", "user", "group",
							"size", "ctime", "name" };
	const int pad[] = { 0, 1, 0, 0, 1, 0, 0 };
	int i;
	int n;

	i = -1;
	while (++i < 7)
	{
		n = get_field(args->pads, field[i])->width - get_field(((t_fileinfo *)elem->content), field[i])->width;
		if(pad[i])
			while(n--)
				ft_putchar(' ');
		ft_putstr(get_field(((t_fileinfo *)elem->content), field[i])->value);
		if(ft_strcmp("name", field[i]) == 0)
		{
			ft_putchar(10);
			break ;
		}
		else if (!pad[i])
			while(n--)
				ft_putchar(' ');
		ft_putchar(' ');
	}
}

void simple_print(t_list *elem, t_padargs *args)
{
	(void) args;
	ft_putstr(get_field(((t_fileinfo *)elem->content), "name")->value);
	ft_putchar('\n');
}

void print_dir(t_list *content, t_options *opts)
{
	t_fileinfo *f;
	t_padargs args;

	f = ft_memalloc(sizeof(*f));
	add_field(f, "name", "");
	if(opts->long_format)
	{
		add_field(f, "perms", "");
		add_field(f, "hl", "");
		add_field(f, "user", "");
		add_field(f, "group", "");
		add_field(f, "size", "");
		add_field(f, "ctime", "");
	}
	args = (t_padargs){f, opts};
	ft_lstiterup(content, (void*)&calc_pads, &args);
	if(opts->long_format)
		ft_lstiterup(content, (void*)&padded_print, &args);
	else
		ft_lstiterup(content, (void*)&simple_print, &args);
}

void render_dir(t_fileinfo *elem, t_options *opts)
{
	t_list *dir_content;
	char *tmp;
	tmp = ft_strdup(set_cwdir(0));
	set_cwdir(elem->name[0] == '/' ? elem->name :
			  ft_strjoin(set_cwdir(0), elem->name));
	if(opts->argc > 1 || opts->recursive)
	{
		ft_putstr(elem->name);
		ft_putchar(':');
	}
	dir_content = get_content(elem->name, opts);
	if(dir_content)
	{
		if(opts->long_format)
		{
			ft_putstr("total ");
			ft_putnbr(get_total_blocks(dir_content));
			ft_putchar(10);
		}
		ft_lstiterup(dir_content, (void*)&bake_fields, opts);
		print_dir(dir_content, opts);
	}
	set_cwdir(tmp);
	free(tmp);
}

void render_list(t_list *elem, t_options *opts)
{
	t_fileinfo *obj;
	const t_renderf rndr_type[] = { &error_handle, &print_gen_file,
									&print_gen_file, 0, &render_dir,
									0, &print_gen_file, 0, &print_gen_file, 0,
									&print_gen_file, 0, &print_gen_file };
	obj = elem->content;
	rndr_type[(obj->info.st_mode & S_IFMT) >> 12](obj, opts);
}
