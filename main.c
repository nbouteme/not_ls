/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 12:56:14 by nbouteme          #+#    #+#             */
/*   Updated: 2015/12/08 16:43:59 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct	s_options
{
	t_list *files;
	int long_format : 1;
	int recursive : 1;
	int hidden : 1;
	int reverse : 1;
	int time_sort : 1;
}				t_options; // remplacer bitfields par pointeur structure d'arguments

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

	opts = sender;


}

int main(int argc, char **argv)
{
	t_options *opts = get_opts(argc, argv);
	t_list *list = ft_lstmapup(opts->files, opts, &read_file_info)
}
