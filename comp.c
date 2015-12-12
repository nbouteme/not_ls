/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 12:56:14 by nbouteme          #+#    #+#             */
/*   Updated: 2015/12/12 12:34:09 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "comp.h"

int type_diff(t_fileinfo *a, t_fileinfo *b)
{
	if((a->info.st_mode & S_IFMT) == S_IFDIR ||
	   (b->info.st_mode & S_IFMT) == S_IFDIR)
		return 1 - (((a->info.st_mode & S_IFMT) != S_IFDIR) * 2);
	return alpha_filecmp(a, b);
}

int time_diff(t_fileinfo *a, t_fileinfo *b)
{
	size_t diff;
#ifdef __APPLE__
# define st_mtim st_mtimespec
#endif
	diff = b->info.st_mtim.tv_sec - a->info.st_mtim.tv_sec;
	if(!diff)
		diff = b->info.st_mtim.tv_nsec - a->info.st_mtim.tv_nsec;
	return diff;
}

int alpha_filecmp(t_fileinfo *a, t_fileinfo *b)
{
	char *x;
	char *y;

	x = a->name;
	if(ft_strcmp(x, ".") == 0)
		return -1;
	y = b->name;
	if(ft_strcmp(y, ".") == 0)
		return 1;

#ifndef __APPLE__
	while(*x && *y && ft_tolower(*x) == ft_tolower(*y))
	{
		++x;
		++y;
	}
	return ft_tolower(*x) - ft_tolower(*y);
#endif
	return ft_strcmp(x, y);
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

int filecmp(const t_list *arg1, const t_list *arg2, t_options *opts)
{
	t_fileinfo *a = arg1->content;
	t_fileinfo *b = arg2->content;
	int diff;

	if(opts->time_sort)
		diff = time_diff(a, b);
	else
		diff = alpha_filecmp(a, b);
	return opts->reverse ? -diff : diff;
}
