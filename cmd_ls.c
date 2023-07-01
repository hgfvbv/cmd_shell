#include "cmd_handle.h"
#include "cmd_ls.h"

void ls_execute(cmd_t *cmd_info)
{
	if(cmd_info->cmd_arg_count != 2)
		return;
	if(cmd_info->cmd_arg_list[1] != NULL)
		ls_file_list(cmd_info->cmd_arg_list[1]);
}

void ls_file_list(const char *dirpath)
{
	DIR *pdir = NULL;
	pdir = opendir(dirpath);
	if(NULL == pdir){
		perror("[ERROR] : opendir() >> ");
		return;
	}
	struct dirent *dir = NULL;
	ls_file_info_t ls_file_info;
	char path[128] = {0};
	while((dir = readdir(pdir)) != NULL)
	{
		if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
			continue;
		memset(&ls_file_info, 0, sizeof(ls_file_info_t));
		make_path_ls(path, dirpath, dir->d_name);	
		get_file_attr(&ls_file_info, path, dir->d_name, DT_LNK == dir->d_type);
		show_ls_file_info(&ls_file_info);
	}	
	closedir(pdir);
}

void make_path_ls(char *path, const char *dirpath, const char *filename)
{
	strcpy(path, dirpath);
	strcat(path, "/");
	strcat(path, filename);
}

void get_file_attr(ls_file_info_t *ls_file_info, const char *path, const char *filename, bool islink)
{
	int ret = -1;
	if(islink)
		ret = lstat(path, &ls_file_info->stat_info);
	else
		ret = stat(path, &ls_file_info->stat_info);
	if(-1 == ret){
		perror("[ERROR] : stat() >> ");
		return;
	}
	get_file_type_ls(ls_file_info);
	if('l' == ls_file_info->type){
		ssize_t res = readlink(path, ls_file_info->link_content, sizeof(ls_file_info->link_content));
		if(-1 == res){
			perror("[ERROR] : readlink() >> ");
			return;
		}
	}
	get_file_permission(ls_file_info);
//	get_file_uname(ls_file_info);
	get_file_gname(ls_file_info);
	get_file_time(ls_file_info);
	strcpy(ls_file_info->name, filename);
}

void get_file_type_ls(ls_file_info_t *ls_file_info)
{
	mode_t mode = ls_file_info->stat_info.st_mode;
	switch(mode & S_IFMT)
	{
		case S_IFBLK:
			ls_file_info->type = 'b';
			break;
		case S_IFCHR:
			ls_file_info->type = 'c';
			break;
		case S_IFDIR:
			ls_file_info->type = 'd';
			break;
		case S_IFIFO:
			ls_file_info->type = 'p';
			break;
		case S_IFLNK:
			ls_file_info->type = 'l';
			break;
		case S_IFREG:
			ls_file_info->type = '-';
			break;
		case S_IFSOCK:
			ls_file_info->type = 's';
			break;
		default:
			break;	
	}
}

void get_file_permission(ls_file_info_t *ls_file_info)
{
	mode_t mode = ls_file_info->stat_info.st_mode;
	char perm[] = {'r', 'w', 'x'};
	int index = 0;
	for(int i = 8; i >= 0; --i)
	{
		if((mode >> i) & 0x1)
			ls_file_info->permission[index] = perm[index % 3];
		else
			ls_file_info->permission[index] = '-';
		index++;	
	}
	ls_file_info->permission[index] = '\0';
}

void get_file_uname(ls_file_info_t *ls_file_info)
{
	struct passwd *pw = getpwuid(ls_file_info->stat_info.st_uid);
	strcpy(ls_file_info->uname, pw->pw_name);
}

void get_file_gname(ls_file_info_t *ls_file_info)
{
	struct group *gr = getgrgid(ls_file_info->stat_info.st_gid);
	strcpy(ls_file_info->gname, gr->gr_name);	
}

void get_file_time(ls_file_info_t *ls_file_info)
{
	char *timestr = ctime(&(ls_file_info->stat_info.st_mtime));
	strcpy(ls_file_info->mtime, timestr);
	ls_file_info->mtime[strlen(timestr) - 1] = '\0';	
}

void show_ls_file_info(ls_file_info_t *ls_file_info)
{
	printf(" %c", ls_file_info->type);
	printf("%s ", ls_file_info->permission);
	printf("%s ", ls_file_info->uname);
	printf("%s ", ls_file_info->gname);
	printf("%ld ", ls_file_info->stat_info.st_size);
	printf("%s ", ls_file_info->mtime);
	if('l' == ls_file_info->type)
		printf("%s -> %s", ls_file_info->name, ls_file_info->link_content);
	else
		printf("%s", ls_file_info->name);
	putchar('\n');
}
