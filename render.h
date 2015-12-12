#ifndef RENDER_H
#define RENDER_H

#include <libft.h>
#include <unistd.h>
#include "options.h"
#include "callback.h"
#include "sys_utils.h"
#include "comp.h"

typedef void(*t_renderf)(t_fileinfo *elem, t_options *opts);
void print_mode_str(t_fileinfo *fi);
void print_fn(t_fileinfo *elem, t_options *opts);
void render_dir(t_fileinfo *elem, t_options *opts);
void render_list(t_list *elem, t_options *opts);
void print_long(t_fileinfo *elem, t_options *opts);

#endif
