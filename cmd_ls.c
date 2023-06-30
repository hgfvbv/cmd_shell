#include "cmd_handle.h"
#include "cmd_ls.h"

int cmd_ls_execute(cmd_t *pcmd)
{
	if(NULL == pcmd)
		return -1;
	if(pcmd->cmd_arg_count != 2){
		fprintf(stderr, "Command argument Error.\n");
		return -1;
	}
	if(pcmd->cmd_arg_list[1] != NULL)
		return cmd_list_directory(pcmd->cmd_arg_list[1]);
	else
		return -1;
}

int cmd_list_directory(const char *dirpath)
{
	DIR *pdir = NULL;
	pdir = opendir(dirpath);
	if(NULL == pdir){
		perror("open(): ");
		return -1;
	}	
	struct dirent *pdirent = NULL;
	file_attr_t attr;
	char path[128] = {0};
	while((pdirent = readdir(pdir)) != NULL)
	{
		if(strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
			continue;
		memset(&attr, 0, sizeof(attr));
		make_path_ls(path, dirpath, pdirent->d_name);
		get_file_attr(&attr, path, pdirent->d_name, DT_LNK == pdirent->d_type);	
		show_file_attributes(&attr);
	}
	closedir(pdir);
	return 0;
}

void make_path_ls(char *path, const char *dirpath, const char *filename)
{
	strcpy(path, dirpath);
	strcat(path, "/");
	strcat(path, filename);
}

int get_file_attr(file_attr_t *pattr, const char *path, const char *filename, bool islink)
{
	int ret = -1;
	if(islink)
		ret = lstat(path, &pattr->fattr_stat_info);
       	else
		ret = stat(path, &pattr->fattr_stat_info);
	if(-1 == ret){
		perror("[ERROR] stat() : ");
		return -1;
	}	
	get_file_type_ls(pattr);	
	if('l' == pattr->f_attr_type){
		ssize_t res = readlink(path, pattr->f_attr_link_content, sizeof(pattr->f_attr_link_content));
		if(-1 == res){
			perror("readlink(): ");
			return -1;
		}
	}
	get_file_permission(pattr);
	get_file_uname(pattr);
	get_file_gname(pattr);
	get_file_last_modify_time(pattr);
	strcpy(pattr->f_attr_name, filename);
	return 0;
}

void get_file_type_ls(file_attr_t *pattr)
{
	mode_t mode = pattr->fattr_stat_info.st_mode;
	printf("[DEBUG] : mode %#o\n", mode);
	switch(mode & S_IFMT)
	{
		case S_IFBLK:
			pattr->f_attr_type = 'b';
			break;
		case S_IFCHR:
			pattr->f_attr_type = 'c';
			break;
		case S_IFDIR:
			pattr->f_attr_type = 'd';
			break;
		case S_IFIFO:
			pattr->f_attr_type = 'p';
			break;
		case S_IFLNK:
			pattr->f_attr_type = 'l';
			break;
		case S_IFREG:
			pattr->f_attr_type = '-';
			break;
		case S_IFSOCK:
			pattr->f_attr_type = 's';
			break;
		default:
			break;	
	}
}

void show_file_attributes(file_attr_t *pattr)
{
	printf(" %c", pattr->f_attr_type);
	printf("%s ", pattr->f_attr_permission);
	printf("%s ", pattr->f_attr_uname);
	printf("%s ", pattr->f_attr_gname);
	printf("%ld ", pattr->fattr_stat_info.st_size);
	printf("%s ", pattr->f_attr_mtime);
	if('l' == pattr->f_attr_type)
		printf("%s -> %s", pattr->f_attr_name, pattr->f_attr_link_content);
	else
		printf("%s", pattr->f_attr_name);
	putchar('\n');
}

void get_file_permission(file_attr_t *pattr)
{
	int index = 0;
	char perm[] = {'r', 'w', 'x'};
	mode_t mode = pattr->fattr_stat_info.st_mode;
	printf("[DEBUG] : mode %#o %#x\n", mode, mode);
	for(int i = 8; i >= 0; --i)
	{
		if((mode >> i) & 0x1)
			pattr->f_attr_permission[index] = perm[index % 3];
		else
			pattr->f_attr_permission[index] = '-';
		index++;	
	}
	pattr->f_attr_permission[index] = '\0';
}

void get_file_uname(file_attr_t *pattr)
{
	printf("[DEBUG] : uid %#X\n", pattr->fattr_stat_info.st_uid);
	struct passwd *pwd = getpwuid(pattr->fattr_stat_info.st_uid);
	printf("[DEBUG] : pwd name %s\n", pwd->pw_name);
	strcpy(pattr->f_attr_uname, pwd->pw_name);	
}

void get_file_gname(file_attr_t *pattr)
{
	struct group *grp = getgrgid(pattr->fattr_stat_info.st_gid);
	strcpy(pattr->f_attr_gname, grp->gr_name);
}

void get_file_last_modify_time(file_attr_t *pattr)
{
	char *timestr = ctime(&(pattr->fattr_stat_info.st_mtime));
	strcpy(pattr->f_attr_mtime, timestr);
	pattr->f_attr_mtime[strlen(timestr) - 1] = '\0';
}
