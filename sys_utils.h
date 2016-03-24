/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/24 09:35:39 by nbouteme          #+#    #+#             */
/*   Updated: 2016/03/24 09:35:40 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_UTILS_H
# define SYS_UTILS_H

# include <libft/std.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>
# include <sys/types.h>
# include <dirent.h>
# include "options.h"
# include "comp.h"
# include "memutils.h"

t_list	*list_content(char *name);
t_list	*get_content(char *name, t_options *opts);
size_t	block_count(t_fileinfo *file);
size_t	get_total_blocks(t_list *file_list);
char	*group_from_id(gid_t gid);
char	*user_from_id(uid_t uid);
void	sum_block_size(t_list *acc, const t_list *op);
char	*set_cwdir(const char *n);
void	exit_somehow_properly(char opts);

#endif
