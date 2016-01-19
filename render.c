/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 12:39:13 by nbouteme          #+#    #+#             */
/*   Updated: 2016/01/19 21:09:51 by nbouteme         ###   ########.fr       */
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
	acl_t acl;

	tmp = ft_strjoin(set_cwdir(0), f->name);
#ifdef __APPLE__
	acl = acl_get_link_np(tmp, ACL_TYPE_ACCESS);
	acl_free(acl);
# else
	acl = acl_extended_file(tmp);
	acl = acl == (void*)-1 ? 0 : acl;
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
		if(opts->flags['l'])
			ft_putstr(" -> ");
		ft = ft_strjoin(set_cwdir(0), elem->name);
		readlink(ft, tmp, 1024);
		ft_putstr(tmp);
		free(ft);
		free(tmp);
	}
	if(opts->flags['l'])
		ft_putstr("\n");
	else
		ft_putchar('\t');
}

char *my_ctime(const time_t *clock)
{
	char *s;

	s = ctime(clock);
#ifdef __APPLE__
	s = ft_strdup(s);
#endif
	return (s);
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
	char *s = my_ctime(&elem->info.st_mtim.tv_sec);
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
	if(S_ISLNK(elem->info.st_mode) && opts->flags['l'])
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
	char *r;
	file = elem->content;
	add_field(file, "name", s = render_fn(file, opts));
	free(s);
	if(!opts->flags['l'])
		return;
	add_field(file, "perms", s = render_perms(file));
	free(s);
	add_field(file, "hl", s = ft_itoa(file->info.st_nlink));
	free(s);
	add_field(file, "user", s = ft_strjoin(user_from_id(file->info.st_uid), " "));
	free(s);
	add_field(file, "group", s = ft_strjoin(group_from_id(file->info.st_gid), " "));
	free(s);
	if (S_ISCHR(file->info.st_mode) || S_ISBLK(file->info.st_mode))
	{
		s = ft_strcat(ft_itoa(major(file->info.st_rdev)), ",");
		ft_memmove(s + 1, s, ft_strlen(s) + 1);
		s[0] = ' ';
		add_field(file, "major", s);
		free(s);
		add_field(file, "size", s = ft_itoa(minor(file->info.st_rdev)));
	}
	else
		add_field(file, "size", s = ft_itoa(file->info.st_size));
	free(s);
	s = my_ctime(&file->info.st_mtim.tv_sec);
	r = s;
	s = ft_strsub(s, 4, ft_strlen(s) - 13);
	free(r);
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
							"major", "size", "ctime", "name" };
	const int pad[] = { 0, 1, 0, 0, 1, 1, 0, 0 };
	int i;
	int n;
	t_field *f;
	t_field *g;
	i = -1;
	while (++i < 8)
	{
		f = get_field(((t_fileinfo *)elem->content), field[i]);
		n = f ? f->width : 0;
		g = get_field(args->pads, field[i]);
		if(!g && !f)
			continue;
		n = (g ? 0 : g->width) - n;
		if(pad[i])
			while(n-- > 0)
				ft_putchar(' ');
		ft_putstr(f ? f->value : "");
		if(ft_strcmp("name", field[i]) == 0)
		{
			ft_putstr("\n");
			break ;
		}
		else if (!pad[i])
			while(n-- > 0)
				ft_putchar(' ');
		ft_putchar(' ');
	}
}

void simple_print(t_list *elem, t_padargs *args)
{
	(void) args;
	ft_putstr(get_field(((t_fileinfo *)elem->content), "name")->value);
	ft_putstr("\n");
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

int one_of_is_blk(t_list *files)
{
	t_list *l;
	t_fileinfo *fi;

	l = files;
	while(l)
	{
		fi = l->content;
		if(fi->info.st_mode & S_IFCHR)
			return 1;
		l = l->next;
	}
	return 0;
}

void print_dir(t_list *content, t_options *opts)
{
	t_fileinfo *f;
	t_padargs args;

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

int g_first = 1;

void render_dir(t_fileinfo *elem, t_options *opts, int first)
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
		if(!g_first)
			ft_putstr("\n");
		ft_putstr(ft_strncmp(cat, "./", 2) == 0 ? cat + 2 : elem->name);
		ft_putstr(":\n");
	}
	if(g_first) g_first = 0;
	free(cat);
	dir_content = get_content(set_cwdir(0), opts);
	if(dir_content)
	{
		if(opts->flags['l'])
		{
			ft_putstr("total ");
			ft_putnbr(get_total_blocks(dir_content));
			ft_putstr("\n");
		}
		ft_lstiterup(dir_content, (void*)&bake_fields, opts);
		print_dir(dir_content, opts);
		if(opts->flags['R'] && (rec_content = ft_lstfilter(dir_content, &keep_dirs)))
			disp(rec_content, opts);
		ft_lstdel(&dir_content, &delete_fileinfo);
	}
	(void)first;
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
	rndr_type[(obj->info.st_mode & S_IFMT) >> 12](obj, opts, !elem->next);
}
