#include <xinu.h>
#include <shprototypes.h>
#include <prodcons_bb.h>

void prodcons_bb(int nargs, char *args[])
{
	int producer_count, consumer_count, producer_iteration, consumer_iteration;  
	char process_number[10];

	producer_count = atoi(args[1]);
	consumer_count = atoi(args[2]);
	producer_iteration = atoi(args[3]);
	consumer_iteration = atoi(args[4]);

	mutex = semcreate(1);

	head = 0;
	tail = 0;

	producer_mutex = semcreate(0);
	consumer_mutex = semcreate(5);

	if (producer_count * producer_iteration != consumer_count * consumer_iteration)
	{
		printf("Iteration Mismatch Error: the number of producer(s) iteration does not match the consumer(s) iteration\n");
		return;
	}

	for (int i = 0; i < producer_count; i++)
	{
		sprintf(process_number, "producer_%d", i);
		resume(create((void *)producer_bb, 4096, 20, process_number, 1, producer_iteration));
	}
	for (int i = 0; i < consumer_count; i++)
	{
		sprintf(process_number, "consumer_%d", i);
		resume(create((void *)consumer_bb, 4096, 20, process_number, 1, consumer_iteration));
	}
}

shellcmd xsh_run(int nargs, char *args[]) {
	if ((nargs == 1) || (strncmp(args[1], "list", 4) == 0)) {
		printf("hello\n");
		printf("list\n");
		printf("prodcons\n");
		printf("prodcons_bb\n");
		return OK;
	}

	args++;
	nargs--;

	if (strncmp(args[0], "hello", 5) == 0){
		resume(create((void *)xsh_hello, 4096, 20, "hello", 2, nargs, args));
	}
	else if (strncmp(args[0], "prodcons_bb", 11) == 0){
		resume(create((void *)prodcons_bb, 4096, 20, "prodcons_bb", 2, nargs, args));
	}
	else if (strncmp(args[0], "prodcons", 8) == 0){ 
		resume(create((void *)xsh_prodcons, 4096, 20, "prodcons", 2, nargs, args));
	}
	else{
		printf("hello\n");
		printf("list\n");
		printf("prodcons\n");
		printf("prodcons_bb\n");
		return OK;
	}
	return 0;
}
