/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 12:56:14 by nbouteme          #+#    #+#             */
/*   Updated: 2015/12/09 15:36:19 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

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
		return (void) (opts->files = ft_lstnew(name, ft_strlen(name)));
	l = opts->files;
	while (l->next)
		l = l->next;
	l->next = ft_lstnew(name, ft_strlen(name));
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
	fi->name = file->content;
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
	return ft_lstnew(fi, sizeof(*fi));
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

void disp(t_list *list, t_options *opts)
{
	size_t block_total;

	block_total = 0;
	if(opts->long_format)
		block_total = get_total_blocks(list);
	printf("Total block %zu\n", block_total);
}

void swap_cptr(char **a, char **b)
{
	char *t;

	t = *a;
	*a = *b;
	*b = t;
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

t_list *copy_elem(t_list *l)
{
	return ft_lstnew(l->content, l->content_size);
}

void ft_lstsplit(t_list *head, t_list **a, t_list **b)
{
	int i;
	t_list *l;
	t_list *p;

	i = 0;
	*a = ft_lstmap(head, &copy_elem);
	l = *a;
	*b = l;
	p = l;
	while(l)
	{
		if(i % 2)
		{
			p = *b;
			*b = (*b)->next;
		}
		l = l->next;
		++i;
	}
	p->next = 0;
}

t_list *lst_merge(t_list *a, t_list *b, int (*cmp)(const t_list *, const t_list *))
{
	t_list *l;
	t_list **ll;

	ll = &l;
	l = 0;
	if (!b)
		return a;
	while(a)
	{
		if(cmp(a, b) > 0)
			swap_cptr((char**)&a, (char **)&b);
		*ll = a;
		ll = &a->next;
		a = *ll;
	}

	*ll = b;
	return l;
}

void ft_lstsort(t_list **head, int (*cmp)(const t_list *, const t_list *))
{
	t_list *a;
	t_list *b;

	if(!*head || !(*head)->next)
		return ;
	ft_lstsplit(*head, &a, &b);
	ft_lstsort(&a, cmp);
	ft_lstsort(&b, cmp);
	*head = lst_merge(a, b, cmp);
}

int alpha_sort(const t_list *a, const t_list *b)
{
	return ft_strcmp(a->content, b->content);
}

void print_lst(t_list *a)
{
	puts(a->content);
}

int main(/*int argc, char **argv*/)
{
/*	t_options *opts = get_opts(argc, argv);
	if (!opts->files)
		add_file(opts, ".");
	t_list *list = ft_lstmapup(opts->files, &read_file_info, opts);
	disp(list, opts);*/
	t_list *a = ft_lstnew("a", 2);
	ft_lstpush(&a, ft_lstnew("t", 2));
	ft_lstpush(&a, ft_lstnew("2", 2));
	ft_lstpush(&a, ft_lstnew("d", 2));
	ft_lstpush(&a, ft_lstnew("3", 2));

	ft_lstsort(&a, &alpha_sort);
	ft_lstiter(a, &print_lst);
}
