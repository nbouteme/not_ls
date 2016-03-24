/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_shit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/24 08:22:40 by nbouteme          #+#    #+#             */
/*   Updated: 2016/03/24 08:29:48 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft/std.h>
#include <unistd.h>
#include <stdlib.h>

void	exit_somehow_properly(char opts)
{
	ft_putstr_fd("ls: illegal option -- ", 2);
	ft_putchar_fd(opts, 2);
	ft_putendl_fd("\nusage: ls [-alrRt1] [file ...]", 2);
	exit(1);
}

char	*set_cwdir(const char *n)
{
	static char	*cwd = 0;
	char		*tmp;

	if (n)
	{
		free(cwd);
		tmp = ft_strdup(n);
		cwd = ft_strjoin(tmp, n[ft_strlen(n) - 1] == '/' ? "" : "/");
		free(tmp);
	}
	return (cwd);
}
