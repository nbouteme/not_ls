/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/24 08:32:07 by nbouteme          #+#    #+#             */
/*   Updated: 2016/03/24 09:32:03 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_utils.h"

char		*user_from_id(uid_t uid)
{
	struct passwd *p;

	p = getpwuid(uid);
	return (ft_strdup(p->pw_name));
}

char		*group_from_id(gid_t gid)
{
	struct group *g;

	g = getgrgid(gid);
	return (ft_strdup(g->gr_name));
}

t_list		*list_content(char *name)
{
	DIR				*dir;
	struct dirent	*r;
	t_list			*ret;

	ret = 0;
	dir = opendir(name);
	if (!dir)
	{
		perror("ls");
		return (0);
	}
	while ((r = readdir(dir)))
		ft_lstadd(&ret, ft_lstnew(r->d_name, ft_strlen(r->d_name) + 1));
	closedir(dir);
	return (ret);
}

t_list		*get_content(char *name, t_options *opts)
{
	t_list *content;
	t_list *ret;
	t_list *tmp;

	content = list_content(name);
	if ((tmp = ft_lstfilterup(content, (void*)&keep_things, opts)))
	{
		ret = ft_lstmapup(tmp, &read_file_info, opts);
		ft_lstdel(&content, &delete_content);
		ft_lstsortup(&ret, (void*)&filecmp, opts);
		ft_lstdel(&tmp, &delete_content);
	}
	else
		ret = 0;
	return (ret);
}

size_t		get_total_blocks(t_list *file_list)
{
	t_list	*ret;
	t_list	init;
	int		n;

	n = 0;
	init = (t_list){ &n, 4, 0 };
	ret = ft_lstreduce(file_list, &init, &sum_block_size);
	n = *(int*)ret->content;
	ft_lstdelone(&ret, &delete_content);
	return (n);
}
