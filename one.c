/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/24 09:27:42 by nbouteme          #+#    #+#             */
/*   Updated: 2016/03/24 09:29:27 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

char	*perms2str(mode_t mode)
{
	char	*str;
	char	*o;
	int		i;
	int		s;
	int		n;

	str = ft_memalloc(11);
	o = str;
	*str++ = g_file_type[(mode & S_IFMT) >> 12];
	s = (mode & 07000) >> 9;
	mode &= 0777;
	i = 3;
	while (i--)
	{
		n = (mode & (07 << (3 * i))) >> (3 * i);
		*str++ = n & 4 ? 'r' : '-';
		*str++ = n & 2 ? 'w' : '-';
		*str = n & 1 ? 'x' : '-';
		if (s & (1 << i))
			*str = (*str == 'x' ? 's' : 'S') + (i == 0);
		++str;
	}
	return (o);
}

int		has_extended(t_fileinfo *f)
{
	int		n;
	char	*tmp;

	tmp = ft_strjoin(set_cwdir(0), f->name);
	tmp = f->name[0] != '/' ? ft_strjoin(set_cwdir(0), f->name)
		: ft_strdup(f->name);
	n = listxattr(tmp, 0, 0, XATTR_NOFOLLOW) > 0;
	free(tmp);
	return (n);
}

int		has_acl(t_fileinfo *f)
{
	char	*tmp;
	acl_t	acl;

	tmp = ft_strjoin(set_cwdir(0), f->name);
	tmp = f->name[0] != '/' ? ft_strjoin(set_cwdir(0), f->name)
		: ft_strdup(f->name);
	acl = acl_get_link_np(tmp, ACL_TYPE_ACCESS);
	acl_free(acl);
	free(tmp);
	return (!!acl);
}

char	*render_perms(t_fileinfo *f)
{
	char *s;
	char *ret;

	s = perms2str(f->info.st_mode);
	ret = ft_strjoin(s, " ");
	ret[10] = has_acl(f) ? '+' : ' ';
	ret[10] = has_extended(f) ? '@' : ret[10];
	free(s);
	return (ret);
}

void	print_mode_str(t_fileinfo *f)
{
	char *s;

	s = perms2str(f->info.st_mode);
	ft_putstr(s);
	free(s);
}
