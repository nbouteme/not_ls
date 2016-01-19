/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 12:56:14 by nbouteme          #+#    #+#             */
/*   Updated: 2016/01/13 16:10:22 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
# include "sys_utils.h"

void exit_somehow_properly(char opts)
{
	ft_putstr_fd("ls: illegal option -- ", 2);
	ft_putchar_fd(opts, 2);
	ft_putendl_fd("\nusage: ls [-alrRt] [file ...]", 2);
	exit(1);
}

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
		if (ft_strindexof("larRt", opt[i]) != -1)
			opts->flags[(unsigned)opt[i]] = 1;
		else
			exit_somehow_properly(opt[i]);
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
	ret->flags = ft_memalloc('z' + 1);
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
	fi->fields = 0;
	fi->name = ft_strdup(file->content);
	fi->real_info = 0;
	t = fi->name[0] != '/' ? ft_strjoin(set_cwdir(0), fi->name) : ft_strdup(fi->name);
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
