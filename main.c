/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 12:56:14 by nbouteme          #+#    #+#             */
/*   Updated: 2015/12/09 15:39:05 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <unistd.h>
#include <grp.h>

typedef struct	s_options
{
	t_list *files;
	size_t dir_count;
	int long_format : 1;
	int recursive : 1;
	int hidden : 1;
	int reverse : 1;
	int time_sort : 1;
}				t_options; // remplacer bitfields par pointeur structure d'arguments

void swap_cptr(char **a, char **b)
{
	char *t;

	t = *a;
	*a = *b;
	*b = t;
}

typedef struct	s_fileinfo
{
	struct stat info;
	int			e;
	char		*name;
	struct stat *real_info;
}				t_fileinfo;

void parse_flag(t_options *opts, char *opt, int *stop)
{
	int l;
	int i;

	l = ft_strlen(opt);
	if (l == 1 && opt[0] == '-')
	{
		*stop = 0;
		return ;
	}
	i = -1;
	while (++i < l)
	{
		opts->long_format |= opt[i] == 'l';
		opts->recursive |= opt[i] == 'R';
		opts->hidden |= opt[i] == 'a';
		opts->reverse |= opt[i] == 'r';
		opts->time_sort |= opt[i] == 't';
	}
}

void add_file(t_options *opts, char *name)
{
	t_list *l;

	if(!opts->files)
		return (void) (opts->files = ft_lstnew(name, ft_strlen(name) + 1));
	l = opts->files;
	while (l->next)
		l = l->next;
	l->next = ft_lstnew(name, ft_strlen(name) + 1);
}

t_options *get_opts(int argc, char **argv)
{
	int i;
	int flag_parse;
	t_options *ret;

	ret = malloc(sizeof(t_options));
	ft_bzero(ret, sizeof(t_options));
	flag_parse = 1;
	i = 0;
	while (++i < argc)
	{
		if (argv[i][0] != '-')
			flag_parse = 0;
		if (flag_parse)
			parse_flag(ret, argv[i] + 1, &flag_parse);
		else
			add_file(ret, argv[i]);
	}
	return ret;
}

t_list *read_file_info(t_list *file, void *sender)
{
	t_options *opts;
	t_fileinfo *fi;
	
	opts = sender;
	fi = malloc(sizeof(*fi));
	fi->name = ft_strdup(file->content);
	fi->real_info = 0;
	fi->e = lstat(fi->name, &fi->info);
	if(!fi->e)
	{
		if ((fi->info.st_mode & S_IFMT) == S_IFLNK)
		{
			fi->real_info = malloc(sizeof(struct stat));
			stat(fi->name, fi->real_info);
		}
		opts->dir_count += (fi->info.st_mode & S_IFMT) == S_IFDIR;
	}
	else
		fi->e = errno;
	return ft_lstnewown(fi, sizeof(*fi));
}

size_t block_count(t_fileinfo *file)
{
	return file->info.st_blocks;
}

void sum_block_size(t_list *acc, const t_list *op)
{
	int *n;

	n = acc->content;
	*n += block_count(op->content);
}

void del_int(void* content, size_t content_size)
{
	free(content);
	(void)content_size;
}

size_t get_total_blocks(t_list *file_list)
{
	t_list *ret;
	t_list init;
	int n;

	n = 0;
	init = (t_list){ &n, 4, 0 };
	ret = ft_lstreduce(file_list, &init, &sum_block_size);
	n = *(int*)ret->content;
	return n;
}

int alpha_filecmp(t_fileinfo *a, t_fileinfo *b);

int type_diff(t_fileinfo *a, t_fileinfo *b)
{
	if((a->info.st_mode & S_IFMT) == S_IFDIR ||
	   (b->info.st_mode & S_IFMT) == S_IFDIR)
		return 1 - (((a->info.st_mode & S_IFMT) != S_IFDIR) * 2);
	return alpha_filecmp(a, b);
}

int time_diff(t_fileinfo *a, t_fileinfo *b)
{
	return b->info.st_mtime - a->info.st_mtime;
}

int alpha_filecmp(t_fileinfo *a, t_fileinfo *b)
{
	return ft_strcmp(a->name, b->name);
}

int cmp_args(t_list *arg1, t_list *arg2, t_options *opts)
{
	t_fileinfo *a = arg1->content;
	t_fileinfo *b = arg2->content;
	int diff;

	if((a->info.st_mode & S_IFMT) != (b->info.st_mode & S_IFMT))
	{
		diff = type_diff(a, b);
		if(!diff)
			diff = alpha_filecmp(a, b);
		return opts->reverse ? -diff : diff;
	}
	if(opts->time_sort)
		diff = time_diff(a, b);
	else
		diff = alpha_filecmp(a, b);
	return opts->reverse ? -diff : diff;
}

typedef void(*t_renderf)(t_fileinfo *elem, t_options *opts);

void render_dir(t_fileinfo *elem, t_options *opts)
{
	(void)elem;
	(void)opts;
}

void render_char(t_fileinfo *elem, t_options *opts)
{
	(void)elem;
	(void)opts;
}

void render_sock(t_fileinfo *elem, t_options *opts)
{
	(void)elem;
	(void)opts;
}

void render_sym(t_fileinfo *elem, t_options *opts)
{
	(void)elem;
	(void)opts;
}

void render_fifo(t_fileinfo *elem, t_options *opts)
{
	(void)elem;
	(void)opts;
}

void print_mode_str(mode_t mode)
{
	const char *file_type = "-pc-d-b---l-s";
	int i;
	int s;
	int n;

	ft_putchar(file_type[(mode & S_IFMT) >> 12]);
	s = mode & 01000;
	mode &= 0777;
	i = 3;
	while (i--)
	{
		n = (mode & (07 << (3 * i))) >> (3 * i);
		ft_putchar(n & 4 ? 'r' : '-');
		ft_putchar(n & 2 ? 'w' : '-');
		if(i == 0 && s)
			ft_putchar(n & 1 ? 't' : 'T');
		else
			ft_putchar(n & 1 ? 'x' : '-');
	}
}

const char *user_from_id(uid_t uid)
{
	struct passwd *p;

	p = getpwuid(uid);
	return p->pw_name;
}

const char *group_from_id(gid_t gid)
{
	struct group *g;

	g = getgrgid(gid);
	return g->gr_name;
}

void print_fn(t_fileinfo *elem, t_options *opts)
{
	// couleurs ici
	ft_putstr(elem->name);
	// couleurs ici
	if(opts->long_format)
		putchar(10);
	else
		putchar('\t');
}

void print_long(t_fileinfo *elem, t_options *opts)
{
	(void)opts;
	print_mode_str(elem->info.st_mode);
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

void render_reg(t_fileinfo *elem, t_options *opts)
{
	if(opts->long_format)
		print_long(elem, opts);
	else
		ft_putstr(elem->name);
}

void render_blk(t_fileinfo *elem, t_options *opts)
{
	(void)elem;
	(void)opts;
}

void error_handle(t_fileinfo *elem, t_options *opts)
{
	(void)opts;
	printf("ls: %s: %s\n", elem->name, strerror(elem->e));
}

void render_list(t_list *elem, t_options *opts)
{
	t_fileinfo *obj;
	const t_renderf rndr_type[] = { &error_handle, &render_fifo,
									&render_char, 0, &render_dir,
									0, &render_blk, 0, &render_reg, 0,
									&render_sym, 0, &render_sock };	
	obj = elem->content;
	rndr_type[(obj->info.st_mode & S_IFMT) >> 12](obj, opts);
}

void disp(t_list *list, t_options *opts)
{
	ft_lstsortup(&list, (void*)&cmp_args, opts);
	ft_lstiterup(list, (void *)&render_list, opts);
}

void sort_string_arr(char **a, int n, int (*op)(char *, char *))
{
	int i;
	int j;

	i = -1;
	while (++i < n)
	{
		j = -1;
		while(++j < n)
			if(op(a[i], a[j]) < 0)
				swap_cptr(&a[i], &a[j]);
	}
}

void delete_content(void *content, size_t size)
{
	(void)size;
	if (content)
		free(content);
}

void delete_fileinfo(void *content, size_t size)
{
	t_fileinfo *s;

	(void)size;
	if (!content)
		return ;
	s = content;
	if(s->name)
		free(s->name);
	if(s->real_info)
		free(s->real_info);
	free(s);
}

int main(int argc, char **argv)
{
	t_options *opts = get_opts(argc, argv);
	if (!opts->files)
		add_file(opts, ".");
	t_list *list = ft_lstmapup(opts->files, &read_file_info, opts);
	disp(list, opts);
	if(list)
		ft_lstdel(&list, &delete_fileinfo);
	if(opts->files)
		ft_lstdel(&opts->files, &delete_content);
	free(opts);
}
