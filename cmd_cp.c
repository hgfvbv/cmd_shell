#include "cmd_handle.h"
#include "cmd_cp.h"

void cp_execute(cmd_t* cmd_info)
{
	if(NULL == cmd_info)
		return;
	cp_file_info_t cp_file_info;
	cp_file_info_parse(&cp_file_info, cmd_info);
	cp_file_info_type(&cp_file_info);
	cp_file_info_dispatch(&cp_file_info);
}

void cp_file_info_parse(cp_file_info_t *cp_file_info, cmd_t *cmd_info)
{
	strcpy(cp_file_info->src, cmd_info->cmd_arg_list[0]);
	strcpy(cp_file_info->dest, cmd_info->cmd_arg_list[1]);
}

void cp_file_info_type(cp_file_info_t *cp_file_info)
{
	enum file_type ftype;
	ftype = get_file_type(cp_file_info->src);
	if(FT_ERROR == ftype || FT_UNKNOW == ftype)
		return;
	cp_file_info->ftype = ftype;
}

enum file_type get_file_type(const char *path)
{
	struct stat stat_info;
	int ret = -1;
	ret = stat(path, &stat_info);
	if(-1 == ret){
		perror("[ERROR] : stat() >> ");
		return FT_ERROR;
	}
	if(S_ISDIR(stat_info.st_mode))
		return FT_DIR;
	else if(S_ISREG(stat_info.st_mode))
		return FT_FILE;	
	return FT_UNKNOW;
}

void cp_file_info_dispatch(cp_file_info_t *cp_file_info)
{
	if(FT_FILE == cp_file_info->ftype)
		cp_file(cp_file_info->src, cp_file_info->dest);
	else if(FT_DIR == cp_file_info->ftype)
		cp_directory(cp_file_info->src, cp_file_info->dest);
}

void cp_file(const char *src, const char *dest)
{
	FILE *fsrc;
	FILE *fdest;
	fsrc = fopen(src, "r");
	fdest = fopen(dest, "w+");
	if(NULL == fsrc || NULL == fdest){
		perror("[ERROR] : fopen() >> ");
		return;
	}
	size_t rbytes, wbytes;
	char buffer[SZ_BUFFER];
	while(1)
	{
		rbytes = fread(buffer, sizeof(char), SZ_BUFFER, fsrc);
		if(rbytes != 0){
			wbytes = fwrite(buffer, sizeof(char), rbytes, fdest);
			if(wbytes != rbytes){
				perror("[ERROR] : fwrite() >> ");
				return;
			}
		}else
			break;
	}	
	fclose(fsrc);
	fclose(fdest);
}

void cp_directory(const char *src, const char *dest)
{
	enum file_type ftype;
	ftype = get_file_type(src);
	if(FT_ERROR == ftype || FT_UNKNOW == ftype){
		perror("[ERROR] : file type >> ");
		return;
	}
	if(FT_DIR == ftype){
		int ret = -1;
		ret = mkdir(dest, 0777);
		if(-1 == ret){
			perror("[ERROR] : mkdir >> ");
			return;
		}
		DIR *dir;
		dir = opendir(src);
		if(NULL == dir){
			perror("[ERROR] : opendir >> ");
			return;
		}
		struct dirent *pdir;
		cp_file_info_t info;
		while(1)
		{
			pdir = readdir(dir);
			if(NULL == pdir)
				break;
			if(strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0)
				continue;
			make_path(&info, src, dest, pdir->d_name);	
			info.ftype = get_file_type(info.src);
			if(FT_DIR == info.ftype)
				cp_directory(info.src, info.dest);
			else if(FT_FILE == info.ftype)
				cp_file(info.src, info.dest);
		}
	}else if(FT_FILE == ftype)
		cp_file(src, dest);	
}

void make_path(cp_file_info_t *cp_file_info, const char *src, const char *dest, const char *filename)
{
	memset(cp_file_info->src, 0, sizeof(cp_file_info->src));
	memset(cp_file_info->dest, 0, sizeof(cp_file_info->dest));
	strcpy(cp_file_info->src, src);
	strcat(cp_file_info->src, "/");
	strcat(cp_file_info->src, filename);
	strcpy(cp_file_info->dest, dest);
	strcat(cp_file_info->dest, "/");
	strcat(cp_file_info->dest, filename);
}
