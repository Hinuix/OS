#include <xinu.h>
#include <prodcons.h>

int n = 0;
int produced, consumed;

shellcmd xsh_prodcons(int nargs, char *args[]) {
	int count = 2000;

	if(nargs > 2){
		printf("Syntax Error");
		return(1);
	}
	else if(nargs == 2){
		count = atoi(args[1]);
	}	

	produced = semcreate(1);
	consumed = semcreate(0);

	resume(create(producer, 1024, 20, "producer", 1, count));
	resume(create(consumer, 1024, 20, "consumer", 1, count));
	return (0);
}
