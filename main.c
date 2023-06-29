#include "cmd_handle.h"

void main(int argc, const char *argv[])
{
	char command[SZ_CMD] = {0};
	while(1)
	{
		printf("TinyShell > ");
		fgets(command, SZ_CMD, stdin);
		command[strlen(command) - 1] = '\0';	
		if(strncmp(command, "quit", 4) == 0)
		{
			printf("GoodBye\n");
			break;	
		}
		cmd_execute(command);
	}	
}
