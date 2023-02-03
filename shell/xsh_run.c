#include <xinu.h>
#include <shprototypes.h>
#include <stdlib.h>
#include <string.h>

shellcmd xsh_run(int nargs, char *args[]) {
	if ((nargs == 1) || (strncmp(args[1], "list", 4) == 0)) {
		printf("hello\n");
		printf("list\n");
		printf("prodcons\n");
		return OK;
	}
	if(strncmp(args[1], "prodcons", 13) == 0) {
    		resume (create((void *) xsh_prodcons, 4096, 20, "prodcons", 2, nargs - 1, &(args[1])));
	}

	 if(strncmp(args[1], "hello", 13) == 0){
		resume (create((void *) xsh_hello, 4096, 20, "hello", 2, nargs - 1, &(args[1])));
	}
	else{
		printf("Syntax:");	
	}
}
