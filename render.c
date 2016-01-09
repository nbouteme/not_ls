/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 12:39:13 by nbouteme          #+#    #+#             */
/*   Updated: 2016/01/09 22:50:10 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

#include <sys/types.h>
#include <sys/acl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/xattr.h>

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

int has_extended(t_fileinfo *f)
{
	int n;
	char *tmp;

	tmp = ft_strjoin(set_cwdir(0), f->name);
	n = listxattr(tmp, 0, 0, XATTR_NOFOLLOW) > 0;
	free(tmp);
	return n;
}


int has_acl(t_fileinfo *f)
{
	char *tmp;
	int fd;
	acl_t acl;

	tmp = ft_strjoin(set_cwdir(0), f->name);
#ifdef __APPLE__
	fd = open(tmp, O_RDONLY);
	acl = acl_get_fd(fd);
	acl_free(acl);
	close(fd);
# else
	fd = acl_extended_file(tmp);
	fd = fd == -1 ? 0 : fd;
	free(tmp);
	return fd;
#endif
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
	char *tmp;
	char *ft;
	// couleurs ici
	ft_putstr(elem->name);
	// couleurs ici

	if(S_ISLNK(elem->info.st_mode))
	{
		tmp = ft_strnew(512);
		ft_putstr(" -> ");
		ft = ft_strjoin(set_cwdir(0), elem->name);
		readlink(ft, tmp, 1024);
		ft_putstr(tmp);
		free(ft);
		free(tmp);
	}

	if(opts->long_format)
		ft_putchar(10);
	else
		ft_putchar('\t');
}

void print_long(t_fileinfo *elem, t_options *opts)
{
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
	char *t;
	char *r;
	char *tmp;
	char *ft;

	(void)opts;//add colors
	r = ft_strdup(elem->name);
	if(S_ISLNK(elem->info.st_mode))
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
	return r;
}

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
	add_field(file, "name", s = render_fn(file, opts));
	free(s);
	if(!opts->long_format)
		return;
	add_field(file, "perms", s = render_perms(file));
	free(s);
	add_field(file, "hl", s = ft_itoa(file->info.st_nlink));
	free(s);
	add_field(file, "user", s = ft_strjoin(user_from_id(file->info.st_uid), " "));
	free(s);
	add_field(file, "group", s = ft_strjoin(group_from_id(file->info.st_gid), " "));
	free(s);
	add_field(file, "size", s = ft_itoa(file->info.st_size));
	free(s);
	s = ctime(&file->info.st_mtim.tv_sec);
	s = ft_strsub(s, 4, ft_strlen(s) - 13);
	add_field(file, "ctime", s);
	free(s);
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

void delete_field(void *elem, size_t size)
{
	t_field *f;

	(void)size;
	f = elem;
	free(f->key);
	free(f->value);
	free(f);
}

void fileinfo_dtor(t_fileinfo *s)
{
	if(s->fields)
		ft_lstdel(&s->fields, &delete_field);
	if(s->name)
		free(s->name);
	if(s->real_info)
		free(s->real_info);
	free(s);
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
	fileinfo_dtor(f);
}

int keep_dirs(t_list *elem)
{
	t_fileinfo *f = elem->content;
	return S_ISDIR(f->info.st_mode);
}

int should_print_name(t_fileinfo *elem, t_options *opts)
{
	t_list *l;

	if(opts->argc > 1)
		return 1;
	l = opts->files;
	while(l)
	{
		if(strcmp(elem->name, l->content) == 0)
			return 0;
		l = l->next;
	}
	return 1;
}

void render_dir(t_fileinfo *elem, t_options *opts)
{
	t_list *dir_content;
	t_list *rec_content;
	char *tmp;
	char *cat;

	tmp = ft_strdup(set_cwdir(0));
	cat = ft_strjoin(set_cwdir(0), elem->name);
	set_cwdir(elem->name[0] == '/' ? elem->name : cat);
	if(should_print_name(elem, opts))
	{
		ft_putstr(ft_strncmp(cat, "./", 2) == 0 ? cat + 2 : elem->name);
		ft_putstr(":\n");
	}
	free(cat);
	dir_content = get_content(set_cwdir(0), opts);
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
		putchar(10);
		if(opts->recursive && (rec_content = ft_lstfilter(dir_content, &keep_dirs)))
			disp(rec_content, opts);
		ft_lstdel(&dir_content, &delete_fileinfo);
	}
	set_cwdir(tmp);
	free(tmp);
}

void render_list(t_list *elem, t_options *opts)
{
	t_fileinfo *obj;
	const t_renderf rndr_type[] = { &error_handle, &print_gen_file,
									&print_gen_file, 0, &render_dir, 0,
									&print_gen_file, 0, &print_gen_file, 0,
									&print_gen_file, 0, &print_gen_file };
	obj = elem->content;
	rndr_type[(obj->info.st_mode & S_IFMT) >> 12](obj, opts);
	if(elem->next)
		putchar(10);
}
