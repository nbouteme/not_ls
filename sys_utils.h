#ifndef SYS_UTILS_H
#define SYS_UTILS_H

#include <libft.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>

#include "options.h"
#include "comp.h"
#include "memutils.h"

t_list *get_content(char *name, t_options *opts);
size_t get_total_blocks(t_list *file_list);
const char *group_from_id(gid_t gid);
const char *user_from_id(uid_t uid);
t_list *list_content(char *name);
void sum_block_size(t_list *acc, const t_list *op);
size_t block_count(t_fileinfo *file);

#endif
