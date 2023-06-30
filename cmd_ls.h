#ifndef __CMD_LS_H__
#define __CMD_LS_H__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#define SZ_LS_NAME 64
#define SZ_LS_TIME 32
#define SZ_LS_PERMISSION 10
#define SZ_LS_LINK_CONTENT 64
typedef struct file_attribute
{
	struct stat fattr_stat_info;
	char f_attr_type;
	char f_attr_uname[SZ_LS_NAME];
	char f_attr_gname[SZ_LS_NAME];
	char f_attr_mtime[SZ_LS_TIME];
	char f_attr_permission[SZ_LS_PERMISSION];
	char f_attr_name[SZ_LS_NAME];
	char f_attr_link_content[SZ_LS_LINK_CONTENT];
}file_attr_t;
extern int cmd_ls_execute(cmd_t*);
extern int cmd_list_directory(const char*);
extern void make_path_ls(char*, const char*, const char*);
extern int get_file_attr(file_attr_t*, const char*, const char*, bool);
extern void get_file_type_ls(file_attr_t*);
extern void show_file_attributes(file_attr_t*);
extern void get_file_permission(file_attr_t*);
extern void get_file_uname(file_attr_t*);
extern void get_file_gname(file_attr_t*);
extern void get_file_last_modify_time(file_attr_t*);
#endif
