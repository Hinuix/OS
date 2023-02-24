#include <xinu.h>
#include <prodcons.h>
#include <prodcons_bb.h>

void producer(int count)
{
    for (int i = 0; i <= count; i++)
    {
        wait(consumed);
        printf("produced : %d\n", i);
        signal(produced);
    }
}

void producer_bb(int id, int count)
{
  for(int i = 0; i < count; i++)
  {
    wait(consumed);
    wait(mutex);

    arr_q[head] = i;
    printf("name : producer_%d, write : %d\n", id, i);
    head = (head + 1) % 5;

  if(head == 5){
    head = 0;
    }

    signal(mutex);
    signal(produced);
  }
  return OK;
}
