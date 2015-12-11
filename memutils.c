#include "memutils.h"

void delete_content(void *content, size_t size)
{
	(void)size;
	if (content)
		free(content);
}

void delete_fileinfo(void *content, size_t size)
{
	t_fileinfo *s;

	(void)size;
	if (!content)
		return ;
	s = content;
	if(s->name)
		free(s->name);
	if(s->real_info)
		free(s->real_info);
	free(s);
}

void del_int(void* content, size_t content_size)
{
	free(content);
	(void)content_size;
}

void swap_cptr(char **a, char **b)
{
	char *t;

	t = *a;
	*a = *b;
	*b = t;
}

void sort_string_arr(char **a, int n, int (*op)(char *, char *))
{
	int i;
	int j;

	i = -1;
	while (++i < n)
	{
		j = -1;
		while(++j < n)
			if(op(a[i], a[j]) < 0)
				swap_cptr(&a[i], &a[j]);
	}
}

