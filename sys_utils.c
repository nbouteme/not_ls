#include "sys_utils.h"

const char *user_from_id(uid_t uid)
{
	struct passwd *p;

	p = getpwuid(uid);
	return p->pw_name;
}

const char *group_from_id(gid_t gid)
{
	struct group *g;

	g = getgrgid(gid);
	return g->gr_name;
}

t_list *list_content(char *name)
{
	DIR *dir;
	struct dirent *r;
	t_list *ret;

	ret = 0;
	dir = opendir(name);
	while((r = readdir(dir)))
		ft_lstadd(&ret, ft_lstnew(r->d_name, ft_strlen(r->d_name)));
	return ret;
}

t_list *get_content(char *name, t_options *opts)
{
	(void)opts;
	t_list *content;
	t_list *ret;
	t_list *tmp;

	content = list_content(name);

	tmp = ft_lstfilterup(content, (void*)&keep_things, opts);
	if (tmp)
	{
		ret = ft_lstmapup(tmp, &read_file_info, opts);
		ft_lstdel(&content, &delete_content);
		ft_lstsortup(&ret, (void*)&filecmp, opts);
	}
	else
		ret = 0;
	return ret;
}

size_t get_total_blocks(t_list *file_list)
{
	t_list *ret;
	t_list init;
	int n;

	n = 0;
	init = (t_list){ &n, 4, 0 };
	ret = ft_lstreduce(file_list, &init, &sum_block_size);
	n = *(int*)ret->content;
	ft_lstdelone(&ret, &delete_content);
	return n;
}


char *set_cwdir(const char *n)
{
	static char *cwd = 0;
	if (!cwd)
		cwd = ft_strdup("./");
	if (n)
	{
		free(cwd);
		cwd = ft_strjoin(ft_strdup(n), n[ft_strlen(n) - 1] == '/' ? "" : "/");
	}
	return cwd;
}
