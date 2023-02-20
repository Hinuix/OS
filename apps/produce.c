#include <xinu.h>
#include <prodcons.h>
#include <prodcons_bb.h>


int arr_q[5];
int head, tail;
sid32 mutex, producer_mutex, consumer_mutex;


void producer(int count)
{
    for (int i = 0; i <= count; i++)
    {
        wait(consumed);
        n = i;
        printf("produced : %d\n", n);
        signal(produced);
    }
}

void producer_bb(int count)
{
  for (int i = 0; i < count; i++)
  {
    wait(consumer_mutex);
    wait(mutex);

    arr_q[head] = i;
    head = (head + 1) % 5;
    printf("name : %s, write : %d\n", proctab[getpid()].prname, i);

    signal(mutex);
    signal(producer_mutex);
  }
}
