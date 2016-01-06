/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 12:56:14 by nbouteme          #+#    #+#             */
/*   Updated: 2015/12/13 11:33:38 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft/std.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "options.h"
#include "callback.h"
#include "memutils.h"

size_t block_count(t_fileinfo *file)
{
#ifdef __APPLE__
	return file->info.st_blocks;
#else
	return file->info.st_blocks >> 1;
#endif
}

void sum_block_size(t_list *acc, const t_list *op)
{
	int *n;

	n = acc->content;
	*n += block_count(op->content);
}

int main(int argc, char **argv)
{
	t_options *opts;
	t_list *list;

	opts = get_opts(argc, argv);;
	set_cwdir("./");
	if (!opts->files)
		add_file(opts, ".");
	list = ft_lstmapup(opts->files, &read_file_info, opts);
	disp(list, opts);
	if(list)
		ft_lstdel(&list, &delete_fileinfo);
	if(opts->files)
		ft_lstdel(&opts->files, &delete_content);
	free(opts);
	free(set_cwdir(0));
}
