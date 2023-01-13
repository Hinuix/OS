#include <xinu.h>
#include <string.h>
#include <stdio.h>


shellcmd xsh_hello(int nargs, char *args[]){
	/* Check argument count */	
	if (nargs < 2){
		printf("too few arguments, try again.\n");
		}
	if (nargs == 2){
		printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
		return 0;
		}
	if (nargs > 2){
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
		}
}
