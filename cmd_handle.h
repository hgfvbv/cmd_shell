#ifndef __CMD_HANDLE_H__
#define __CMD_HANDLE_H__
//#define DEBUG
#define CMD_SIZE 128
#define SZ_NAME 8
#define SZ_COUNT 2
#define SZ_ARG 32
#include <stdio.h>
#include <string.h>
typedef struct
{
	char cmd_name[SZ_NAME];
	char cmd_arg_list[SZ_COUNT][SZ_ARG];
	int cmd_arg_count;
}cmd_t;
extern void cmd_execute(char*);
extern void init_command(cmd_t*);
extern void print_command(cmd_t*);
extern void cmd_info_parse(char*, cmd_t*);
extern void cmd_dispatch(cmd_t*);
#endif
