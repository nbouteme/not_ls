/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 12:56:14 by nbouteme          #+#    #+#             */
/*   Updated: 2015/12/12 17:31:48 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONS_H
# define OPTIONS_H

# include <libft/std.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <sys/stat.h>

# ifdef __APPLE__
#  define st_mtim st_mtimespec
# endif

typedef struct	s_options
{
	t_list *files;
	size_t argc;
	int long_format : 1;
	int recursive : 1;
	int hidden : 1;
	int reverse : 1;
	int time_sort : 1;
}				t_options; // remplacer bitfields par pointeur structure d'arguments

typedef struct	s_fileinfo
{
	t_list		*fields;
	char		*name;
	struct stat *real_info;
	int			e;
	struct stat info;
}				t_fileinfo;

typedef struct	s_field
{
	char	*key;
	char	*value;
	int		width;
}				t_field;

t_list *read_file_info(t_list *file, void *sender);
t_options *get_opts(int argc, char **argv);
void add_file(t_options *opts, char *name);
void parse_flag(t_options *opts, char *opt, int *stop);

#endif
