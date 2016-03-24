/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/24 09:35:50 by nbouteme          #+#    #+#             */
/*   Updated: 2016/03/24 09:36:20 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <libft/std.h>
# include <unistd.h>
# include "options.h"
# include "callback.h"
# include "sys_utils.h"
# include "comp.h"
# include <sys/types.h>
# include <sys/acl.h>
# include <fcntl.h>
# include <sys/xattr.h>
# define ABS(x) ((x) < 0 ? -(x) : (x))

typedef void(*t_renderf)(t_fileinfo *elem, t_options *opts);

typedef struct	s_padargs
{
	t_fileinfo	*pads;
	t_options	*opts;
}				t_padargs;

void			render_list(t_list *elem, t_options *opts);
void			render_dir(t_fileinfo *elem, t_options *opts);
int				should_print_name(t_fileinfo *elem, t_options *opts);
int				should_print_name(t_fileinfo *elem, t_options *opts);
int				keep_dirs(t_list *elem);
void			print_dir(t_list *content, t_options *opts);
int				one_of_is_blk(t_list *files);
void			fileinfo_dtor(t_fileinfo *s);
void			delete_field(void *elem, size_t size);
void			simple_print(t_list *elem, t_padargs *args);
void			padded_print(t_list *elem, t_padargs *args);
void			calc_pads(t_list *elem, t_padargs *args);
void			bake_fields(t_list *elem, t_options *opts);
t_field			*get_field(t_fileinfo *elem, const char *key);
void			add_field(t_fileinfo *elem, const char *key, char *value);
char			*render_fn(t_fileinfo *elem, t_options *opts);
void			print_long(t_fileinfo *elem, t_options *opts);
char			*my_ctime(const time_t *clock);
void			print_fn(t_fileinfo *elem, t_options *opts);
void			print_mode_str(t_fileinfo *f);
char			*render_perms(t_fileinfo *f);
int				has_acl(t_fileinfo *f);
int				has_extended(t_fileinfo *f);
char			*perms2str(mode_t mode);
void			print_mode_str(t_fileinfo *fi);
void			print_fn(t_fileinfo *elem, t_options *opts);
void			render_dir(t_fileinfo *elem, t_options *opts);
void			render_list(t_list *elem, t_options *opts);
void			print_long(t_fileinfo *elem, t_options *opts);
void			fileinfo_dtor(t_fileinfo *s);

extern int			g_first;
extern int			g_errored;
extern const char	*g_file_type;
extern const char	*g_field[];
extern const int	g_pad[];

#endif
