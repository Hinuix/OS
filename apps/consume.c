#include <xinu.h>
#include <prodcons.h>
#include <stdio.h>

void consumer(int count, sid32 prod_sem, sid32 cons_sem) {
  for(int i = 0; i <= count; i++) {
    wait(cons_sem);
    printf("consumed : %d\n", n);
    signal(prod_sem);
  }
}

void consumer_bb(int idx, int count, sid32 prod_sem, sid32 cons_sem) {
  for(int i = 0; i < count; i++) {
    wait(cons_sem);
    wait(mutex_bb);
    printf("name : consumer_%d, read : %d\n", idx, arr_q[arr_tail]);
    arr_tail = (arr_tail + 1) % 5;
    signal(mutex_bb);
    signal(prod_sem);
  }
}