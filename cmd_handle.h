#ifndef __CMD_HANDLER_H__
#define __CMD_HENDLER_H__
//#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SZ_CMD 256
#define SZ_NAME 8
#define SZ_COUNT 2
#define SZ_ARG 32
typedef struct
{
	char cmd_name[SZ_NAME];
	char cmd_arg_list[SZ_COUNT][SZ_ARG];
	int cmd_arg_count;
}cmd_t;
extern int cmd_execute(char*);
extern void init_command_struct(cmd_t*);
extern void print_command_info(cmd_t*);
extern int cmd_parse(char*, cmd_t*);
extern int cmd_dispatch(cmd_t*);
#endif
