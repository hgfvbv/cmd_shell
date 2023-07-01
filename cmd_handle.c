#include "cmd_handle.h"
#include "cmd_cp.h"
#include "cmd_ls.h"

void cmd_execute(char *command)
{
	if(NULL == command)
		return;
	cmd_t cmd_info;
	init_command(&cmd_info);
	cmd_info_parse(command, &cmd_info);
#ifdef DEBUG
	print_command(&cmd_info);
#endif
	cmd_dispatch(&cmd_info);
}

void init_command(cmd_t *cmd_info)
{
	memset(cmd_info->cmd_name, 0, sizeof(SZ_NAME));
	for(int i = 0; i < SZ_COUNT; ++i)
		memset(cmd_info->cmd_arg_list[i], 0, sizeof(SZ_ARG));
	cmd_info->cmd_arg_count = 0;
}

void print_command(cmd_t *cmd_info)
{
	printf("================\n");
	printf("[DEBUG] : cmd_name >> %s\n", cmd_info->cmd_name);
	printf("[DEBUG] : cmd_arg_count >> %d\n", cmd_info->cmd_arg_count);
	printf("[DEBUG] : cmd_arg_list >> ");
	for(int i = 0; i < SZ_COUNT; ++i)
		printf("%s ", cmd_info->cmd_arg_list[i]);
	putchar('\n');
	printf("================\n");
}

void cmd_info_parse(char *command, cmd_t *cmd_info)
{
	char *parse_str;
	parse_str = strtok(command, " ");
	strcpy(cmd_info->cmd_name, parse_str);
	int index = 0;
	while((parse_str = strtok(NULL, " ")) != NULL)
	{
		strcpy(cmd_info->cmd_arg_list[index], parse_str);	
		index++;	
	}	
	cmd_info->cmd_arg_count = index;
}

void cmd_dispatch(cmd_t *cmd_info)
{
	if(strcmp(cmd_info->cmd_name, "cp") == 0)
		cp_execute(cmd_info);
	else if(strcmp(cmd_info->cmd_name, "ls") == 0)
		ls_execute(cmd_info);
	else
		printf("%s unextened\n", cmd_info->cmd_name);
}
