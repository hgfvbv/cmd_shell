#include "cmd_handle.h"
#include "cmd_cp.h"
#include "cmd_ls.h"

void init_command_struct(cmd_t *pcmd)
{
	memset(pcmd->cmd_name, 0, sizeof(SZ_NAME));
	for(int i = 0; i < SZ_COUNT; ++i)
	{
		memset(pcmd->cmd_arg_list[i], 0, sizeof(SZ_ARG));	
	}
	pcmd->cmd_arg_count = 0;
}

void print_command_info(cmd_t *pcmd)
{
	printf("==================\n");
	printf("[DEBUG] : cmd name : < %s >\n", pcmd->cmd_name);
	printf("[DEBUG] : cmd arg count : < %d >\n", pcmd->cmd_arg_count);
	printf("[DEBUG] : cmd arg list : ");
	for(int i = 0; i < SZ_COUNT; ++i)
	{
		printf(" %s ", pcmd->cmd_arg_list[i]);	
	}
	printf("\n==================\n");
}

int cmd_execute(char *cmd_str)
{
	if(NULL == cmd_str)
		return -1;
#ifdef DEBUG
	printf("[DEBUG] : cmd string : < %s >\n", cmd_str);
#endif
	cmd_t* command = (cmd_t*)malloc(sizeof(cmd_t));
	init_command_struct(command);
	int ret = -1;
	ret = cmd_parse(cmd_str, command);
	if(-1 == ret)
		return -1;
#ifdef DEBUG
	print_command_info(command);
#endif
	ret = cmd_dispatch(command);
	if(-1 == ret)
		return -1;
	return 1;
}

int cmd_parse(char* cmd_str, cmd_t *pcmd)
{
	if(NULL == cmd_str || NULL == pcmd)
		return -1;
	char *p_cmd_name = NULL;
	char *p_cmd_arg = NULL;
	int index = 0;
	p_cmd_name = strtok(cmd_str, " ");
	strcpy(pcmd->cmd_name, p_cmd_name);
	while(1)
	{
		p_cmd_arg = strtok(NULL, " ");
		if(NULL == p_cmd_arg)
			break;	
		strcpy(pcmd->cmd_arg_list[index++], p_cmd_arg);
	}
	pcmd->cmd_arg_count = index;
	return 0;
}

int cmd_dispatch(cmd_t *pcmd)
{
	if(NULL == pcmd)
		return -1;
	if(strncmp(pcmd->cmd_name, "ls", 2) == 0){
		cmd_ls_execute(pcmd);	
	}else if(strncmp(pcmd->cmd_name, "cp", 2) == 0){
		cmd_cp_execute(pcmd);
	}else{
		printf("sorry, %s unextended.\n", pcmd->cmd_name);
	}	
	return 0;
}
