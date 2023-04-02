#include <xinu.h>
#include <prodcons.h>
#include <stdio.h>

void producer(int count, sid32 prod_sem, sid32 cons_sem) {
  for(int i = 0; i <= count; i++) {
    wait(prod_sem);
    n = i;
    printf("produced : %d\n", n);
    signal(cons_sem);
  }
}

void producer_bb(int idx, int count, sid32 prod_sem, sid32 cons_sem) {
  for(int i = 0; i < count; i++) {
    wait(prod_sem);
    wait(mutex_bb);
    n = i;
    arr_q[arr_head] = n;
    arr_head = (arr_head + 1) % 5;
    printf("name : producer_%d, write : %d\n", idx, n);
    signal(mutex_bb);
    signal(cons_sem);
  }
}