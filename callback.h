#ifndef CALLBACK_H
#define CALLBACK_H

#include "options.h"
#include "render.h"
#include "comp.h"

void	error_handle(t_fileinfo *elem, t_options *opts, int last);
void	print_gen_file(t_fileinfo *elem, t_options *opts, int last);
void	print_list_help(t_list *elem, t_options *opts);
void	disp(t_list *list, t_options *opts);

int		keep_things(t_list *elem, t_options *opts);

#endif
