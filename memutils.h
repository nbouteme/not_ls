/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memutils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/24 09:32:30 by nbouteme          #+#    #+#             */
/*   Updated: 2016/03/24 09:33:15 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMUTILS_H
# define MEMUTILS_H

# include "render.h"

void sort_string_arr(char **a, int n, int (*op)(char *, char *));
void delete_fileinfo(void *content, size_t size);
void del_int(void *content, size_t content_size);
void delete_content(void *content, size_t size);
void swap_cptr(char **a, char **b);

#endif
