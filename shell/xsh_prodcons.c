#include <xinu.h>
#include <prodcons.h>
#include <stdlib.h>

int n = 0;
int produced, consumed;
extern sid32 produced;
extern sid32 consumed;


shellcmd xsh_prodcons(int nargs, char *args[]) {
	
	int count = 200;

	if(nargs > 2 || count == 0){
		printf("Syntax: run prodcons [counter] \n");
		return 1;
	}

	else if(nargs == 2){
		count = atoi(args[1]);
	}	

	produced = semcreate(0);
	consumed = semcreate(1);

	resume(create(producer, 1024, 20, "producer", 1, count));
	resume(create(consumer, 1024, 20, "consumer", 1, count));
	return 0;
}