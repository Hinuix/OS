#include <xinu.h>
#include <prodcons.h>

void producer(int count) {
	for(int i = 0; i <= count; i++){
	wait(consumed);
	n = i;
		printf("produced : %d\n", n);
	signal(produced);
	}
}
