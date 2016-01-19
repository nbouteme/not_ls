#include "callback.h"

void print_list_help(t_list *elem, t_options *opts)
{
	print_gen_file(elem->content, opts, 1);
}

int keep_things(t_list *elem, t_options *opts)
{
	char *name;

	name = elem->content;
	return (name[0] != '.' || opts->flags['a']);
}

void error_handle(t_fileinfo *elem, t_options *opts, int last)
{
	(void)opts;
	(void)last;
	ft_putstr("ls: ");
	errno = elem->e;
	perror(elem->name);
}

void disp(t_list *list, t_options *opts)
{
	ft_lstsortup(&list, (void*)&cmp_args, opts);
	ft_lstiterup(list, (void *)&render_list, opts);
}

void print_gen_file(t_fileinfo *elem, t_options *opts, int last)
{
	(void)last;
	if(opts->flags['l'])
		print_long(elem, opts);
	else
		ft_putendl(elem->name);
}
