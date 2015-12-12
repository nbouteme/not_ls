/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 12:56:14 by nbouteme          #+#    #+#             */
/*   Updated: 2015/12/12 17:31:53 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
# include "sys_utils.h"

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
		{
			add_file(ret, argv[i]);
			ret->argc++;
		}
	}
	return ret;
}

t_list *read_file_info(t_list *file, void *sender)
{
	t_options *opts;
	t_fileinfo *fi;
	char *t;

	opts = sender;// why did i need that ?
	(void)opts;
	fi = malloc(sizeof(*fi));
	fi->name = ft_strdup(file->content);
	fi->real_info = 0;
	t = ft_strjoin(set_cwdir(0), fi->name);
	fi->e = lstat(t, &fi->info);
	if(fi->e)
		fi->e = errno;
	else if ((fi->info.st_mode & S_IFMT) == S_IFLNK)
	{
		fi->real_info = malloc(sizeof(struct stat));
		stat(t, fi->real_info);
	}
	free(t);
	return ft_lstnewown(fi, sizeof(*fi));
}
