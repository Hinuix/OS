#include <xinu.h>
#include <prodcons.h>
#include <prodcons_bb.h>


int arr_q[5];
int head, tail;
sid32 mutex;


void consumer(int count){
	for (int i = 0; i <= count; i++){
		wait(produced);
		printf("consumed : %d\n", n);
		signal(consumed);
	}
}

void consumer_bb(int id, int count)
{
	int i, temp;
	for (i = 0; i < count; i++)
	{
		wait(produced);
		wait(mutex);
		temp = arr_q[tail];
		printf("name : consumer_%d, read : %d\n", id, temp);
		tail = (tail + 1) % 5;
		signal(mutex);
		signal(consumed);

		if(tail == 5){
			tail = 0;
			}
	}
	return OK;
}
