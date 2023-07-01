#ifndef __CMD_LS_H__
#define __CMD_LS_H__
#define SZ_LS_NAME 64
#define SZ_LS_PERMISSION 8
#define SZ_LS_TIME 32
#define SZ_LS_LINK_CONTENT 64
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
typedef struct
{
	struct stat stat_info;
	char type;
	char permission[SZ_LS_PERMISSION];
	char uname[SZ_LS_NAME];
	char gname[SZ_LS_NAME];
	char mtime[SZ_LS_TIME];
	char name[SZ_LS_NAME];
	char link_content[SZ_LS_LINK_CONTENT];	
}ls_file_info_t;
extern void ls_execute(cmd_t*);
extern void ls_file_list(const char*);
extern void make_path_ls(char*, const char*, const char*);
extern void get_file_attr(ls_file_info_t*, const char*, const char*, bool);
extern void get_file_type_ls(ls_file_info_t*);
extern void get_file_permission(ls_file_info_t*);
extern void get_file_uname(ls_file_info_t*);
extern void get_file_gname(ls_file_info_t*);
extern void get_file_time(ls_file_info_t*);
extern void show_ls_file_info(ls_file_info_t*);
#endif
