#include <xinu.h>
#include <prodcons.h>

int arr_q[5];
int head, tail;
sid32 mutex, producer_mutex, consumer_mutex;


void consumer(int count){
	for (int i = 0; i <= count; i++){
		wait(produced);
		printf("consumed : %d\n", n);
		signal(consumed);
	}
}

void consumer_bb(int count)
{
	char *process_name = proctab[getpid()].prname;
	for (int i = 0; i < count; i++)
	{
		wait(producer_mutex);
		wait(mutex);

		int temp = arr_q[tail];
		tail = (tail + 1) % 5;
		printf("name : %s, read : %d\n", process_name, temp);

		signal(mutex);
		signal(consumer_mutex);
	}
}
