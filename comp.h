/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbouteme <nbouteme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 12:56:14 by nbouteme          #+#    #+#             */
/*   Updated: 2015/12/09 15:39:05 by nbouteme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMP_H
# define COMP_H

# include "options.h"

int type_diff(t_fileinfo *a, t_fileinfo *b);
int time_diff(t_fileinfo *a, t_fileinfo *b);
int alpha_filecmp(t_fileinfo *a, t_fileinfo *b);
int cmp_args(t_list *arg1, t_list *arg2, t_options *opts);
int filecmp(const t_list *arg1, const t_list *arg2, t_options *opts);

#endif
