#ifndef __CMD_CP_H__
#define __CMD_CP_H__
#define SZ_PATH 128
#define SZ_BUFFER 1024
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
enum file_type
{
	FT_DIR = 0,
	FT_FILE = 1,
	FT_ERROR = 2,
	FT_UNKNOW = 3,
};
typedef struct
{
	enum file_type ftype;
	char src[SZ_PATH];
	char dest[SZ_PATH];
}cp_file_info_t;
extern void cp_execute(cmd_t*);
extern void cp_file_info_parse(cp_file_info_t*, cmd_t*);
extern void cp_file_info_type(cp_file_info_t*);
extern enum file_type get_file_type(const char*);
extern void cp_file_info_dispatch(cp_file_info_t*);
extern void cp_file(const char*, const char*);
extern void cp_directory(const char*, const char*);
extern void make_path(cp_file_info_t*, const char*, const char*, const char*);
#endif
