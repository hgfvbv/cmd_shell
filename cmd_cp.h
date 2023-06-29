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
	FT_DIR = 0, // 目录
	FT_FILE = 1, // 普通文件
	FT_ERROR = 2, // 出错
	FT_UNKNOW = 3, // 文件类型未知	
};
typedef struct
{
	enum file_type ftype;
	char src_path[SZ_PATH];
	char dest_path[SZ_PATH];	
}cp_file_info_t;
extern int cmd_cp_execute(cmd_t*);
extern int cmd_cp_parse_path(cp_file_info_t*, cmd_t*);
extern enum file_type get_file_type(const char*);
extern int cmd_cp_parse_type(cp_file_info_t*);
extern int cmd_cp_dispatch(cp_file_info_t*);
extern int cmd_cp_file(const char*, const char*);
extern int cmd_cp_directory(const char*, const char*);
extern void make_path(cp_file_info_t*, const char*, const char*, const char*);
#endif
