#include "cmd_handle.h"
#include "cmd_cp.h"

int cmd_cp_execute(cmd_t *pcmd)
{
	if(NULL == pcmd || pcmd->cmd_arg_count != 2)
		return -1;
	cp_file_info_t *pfileinfo = NULL;
	int ret = -1;
	pfileinfo = (cp_file_info_t*)malloc(sizeof(cp_file_info_t));
	ret = cmd_cp_parse_path(pfileinfo, pcmd);
	if(-1 == ret)
		return -1;
	ret = cmd_cp_parse_type(pfileinfo);
	if(-1 == ret)
		return -1;
	ret = cmd_cp_dispatch(pfileinfo);
	if(-1 == ret)
		return -1;
	return 0;
}

int cmd_cp_parse_path(cp_file_info_t *pfileinfo, cmd_t *pcmd)
{
	if(NULL == pfileinfo || NULL == pcmd)
		return -1;
	strcpy(pfileinfo->src_path, pcmd->cmd_arg_list[0]);
	strcpy(pfileinfo->dest_path, pcmd->cmd_arg_list[1]);	
#ifdef DEBUG
	printf("src path : < %s >\n", pfileinfo->src_path);
	printf("dest path : < %s >\n", pfileinfo->dest_path);
#endif
	return 0;
}

int cmd_cp_parse_type(cp_file_info_t *pfileinfo)
{
	if(NULL == pfileinfo)
		return -1;
	int ret = -1;
	enum file_type ftype;
	ftype = get_file_type(pfileinfo->src_path);
	if(FT_ERROR == ftype || FT_UNKNOW == ftype)
		return -1;
	else
		pfileinfo->ftype = ftype;
	return 0;
}

enum file_type get_file_type(const char *path)
{
	if(NULL == path)
		return FT_ERROR;
	int ret = -1;
	struct stat stat_info;	
	ret = stat(path, &stat_info);
	if(-1 == ret)
	{
		perror("stat(): ");
		return FT_ERROR;	
	}
	if(S_ISDIR(stat_info.st_mode))
		return FT_DIR;
	else if(S_ISREG(stat_info.st_mode))
		return FT_FILE;
	return FT_UNKNOW;
}

int cmd_cp_dispatch(cp_file_info_t *pfileinfo)
{
	if(FT_FILE == pfileinfo->ftype)
		return cmd_cp_file(pfileinfo->src_path, pfileinfo->dest_path);
	else if(FT_DIR == pfileinfo->ftype)
		return cmd_cp_directory(pfileinfo->src_path, pfileinfo->dest_path);
}

int cmd_cp_file(const char *src, const char *dest)
{
	if(NULL == src || NULL == dest)
		return -1;
	FILE *fp_src = NULL, *fp_dest = NULL;
	size_t rbytes = 0, wbytes = 0;
	char buffer[SZ_BUFFER] = {0};
#ifdef DEBUG
	printf("[DEBUG] : %s -----> %s\n", src, dest);
#endif
	fp_src = fopen(src, "r");
	fp_dest = fopen(dest, "w+");
	if(NULL == fp_src || NULL == fp_dest)
		return -1;
	while(1)
	{
		rbytes = fread(buffer, sizeof(char), SZ_BUFFER, fp_src);
		if(rbytes != 0)
		{
			wbytes = fwrite(buffer, sizeof(char), rbytes, fp_dest);
			if(wbytes != rbytes)
			{
				perror("fwrite(): ");
				return -1;
			}	
		}else{
			break;
		}	
	}
	fclose(fp_src);
	fclose(fp_dest);
	return 0;
}

int cmd_cp_directory(const char *src, const char *dest)
{
	if(NULL == src || NULL == dest)
		return -1;
	enum file_type ftype;
	ftype = get_file_type(src);
	if(FT_ERROR == ftype || FT_UNKNOW == ftype)
		return -1;
	if(FT_DIR == ftype)
	{
		int ret = -1;
		ret = mkdir(dest, 0777);
		if(-1 == ret){
			perror("mkdir(): ");
			return -1;
		}
		DIR *pdir = NULL;
		pdir = opendir(src);
		if(NULL == pdir){
			perror("opendir(): ");
			return -1;
		}
		struct dirent *pdirent = NULL;
		cp_file_info_t info;
		while(1)
		{
			pdirent = readdir(pdir);
			if(NULL == pdirent)
				break;
			if(strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
				continue;
			make_path(&info, src, dest, pdirent->d_name);
#ifdef DEBUG
	printf("[DEBUG] : dest path : %s\n", info.dest_path);
#endif
			info.ftype = get_file_type(src);
			if(FT_DIR == info.ftype)
				cmd_cp_directory(info.src_path, info.dest_path);
			else if(FT_FILE == info.ftype)
				cmd_cp_file(info.src_path, info.dest_path);	
		}	
	}else if(FT_FILE == ftype)
		cmd_cp_file(src, dest);
	else
		return 0;
}

void make_path(cp_file_info_t *pinfo, const char *spath, const char *dpath, const char *filename)
{
	memset(pinfo->src_path, 0, sizeof(pinfo->src_path));
	memset(pinfo->dest_path, 0, sizeof(pinfo->dest_path));
	strcpy(pinfo->src_path, spath);
	strcat(pinfo->src_path, "/");
	strcat(pinfo->src_path, filename);
	strcpy(pinfo->dest_path, dpath);
	strcat(pinfo->dest_path, "/");
	strcat(pinfo->dest_path, filename);
}
