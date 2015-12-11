#include "render.h"

void print_mode_str(mode_t mode)
{
	const char *file_type = "-pc-d-b---l-s";
	int i;
	int s;
	int n;

	ft_putchar(file_type[(mode & S_IFMT) >> 12]);
	s = mode & 01000;
	mode &= 0777;
	i = 3;
	while (i--)
	{
		n = (mode & (07 << (3 * i))) >> (3 * i);
		ft_putchar(n & 4 ? 'r' : '-');
		ft_putchar(n & 2 ? 'w' : '-');
		if(i == 0 && s)
			ft_putchar(n & 1 ? 't' : 'T');
		else
			ft_putchar(n & 1 ? 'x' : '-');
	}
}

void print_fn(t_fileinfo *elem, t_options *opts)
{
	// couleurs ici
	ft_putstr(elem->name);
	// couleurs ici
	if(opts->long_format)
		ft_putchar(10);
	else
		ft_putchar('\t');
}

void print_long(t_fileinfo *elem, t_options *opts)
{
	(void)opts;
	print_mode_str(elem->info.st_mode);
	ft_putchar(32);
	ft_putnbr(elem->info.st_nlink);
	ft_putchar(32);
	ft_putstr(user_from_id(elem->info.st_uid));
	ft_putchar(32);
	ft_putstr(group_from_id(elem->info.st_gid));
	ft_putchar(32);
	ft_putnbr(elem->info.st_size);
	ft_putchar(32);
	char *s = ctime(&elem->info.st_mtim.tv_sec);
	write(1, s, ft_strlen(s) - 1);
	ft_putchar(32);
	print_fn(elem, opts);
}

void render_dir(t_fileinfo *elem, t_options *opts)
{
	t_list *dir_content;

	if(opts->argc > 1 || opts->recursive)
	{
		ft_putstr(elem->name);
		ft_putchar(':');
	}
	dir_content = get_content(elem->name, opts);
	if(opts->long_format)
	{
		ft_putstr("total ");
		ft_putnbr(get_total_blocks(dir_content));
		ft_putchar(10);
	}
	//ft_lstiter(dir_content, (void*)&print_list);
	if(dir_content)
		ft_lstiterup(dir_content, (void*)&print_list_help, opts);
}

void render_list(t_list *elem, t_options *opts)
{
	t_fileinfo *obj;
	const t_renderf rndr_type[] = { &error_handle, &print_gen_file,
									&print_gen_file, 0, &render_dir,
									0, &print_gen_file, 0, &print_gen_file, 0,
									&print_gen_file, 0, &print_gen_file };	
	obj = elem->content;
	rndr_type[(obj->info.st_mode & S_IFMT) >> 12](obj, opts);
}
