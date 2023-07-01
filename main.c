#include "cmd_handle.h"

void main(int argc, const char *argv[])
{
	char command[CMD_SIZE] = {0};
	while(1)
	{
		printf("TinyShell > ");
		fgets(command, CMD_SIZE, stdin);
		command[strlen(command) - 1] = '\0';
		if(strncmp(command, "quit", 2) == 0){
			printf("GoodBye\n");
			break;
		}	
		cmd_execute(command);
	}	
}
