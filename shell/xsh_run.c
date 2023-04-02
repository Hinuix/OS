#include <xinu.h>
#include <shprototypes.h>
#include <prodcons_bb.h>
#include <prodcons.h>
#include <string.h>
#include <heap.h>

int arr_q[5];
int arr_head = 0;
int arr_tail = 0;
sid32 mutex_bb;

void prodcons_bb(int nargs, char *args[]) {

  int prod_n, cons_m, prod_i, cons_j;
  prod_n = atoi(args[1]);
  cons_m = atoi(args[2]);
  prod_i = atoi(args[3]);
  cons_j = atoi(args[4]);

  if (prod_n * prod_i != cons_m * cons_j) {
    printf("Error!\n");
  }

  mutex_bb = semcreate(1);
  sid32 prod_sem = semcreate(5);
  sid32 cons_sem = semcreate(0);
  
  for (int i = 0; i < prod_n; i++) {
    resume(create(producer_bb, 1024, 20, "producer_bb", 4, i, prod_i, prod_sem, cons_sem));
  }
  for (int j = 0; j < cons_m; j++) {
    resume(create(consumer_bb, 1024, 20, "consumer_bb", 4, j, cons_j, prod_sem, cons_sem));
  }

  return 0;
}

shellcmd xsh_run(int nargs, char *args[]) {
	if ((nargs == 1) || (strncmp(args[1], "list", 6) == 0)) {
		printf("hello\n");
		printf("list\n");
		printf("prodcons\n");
		printf("prodcons_bb\n");
		printf("futest\n");
		printf("memtest\n");
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
	else if (strncmp(args[0], "futest", 6) == 0){ 
		resume(create((void *)xsh_futest, 4096, 20, "futest", 2, nargs, args));
	}
	else if (strncmp(args[0], "memtest", 7) == 0){
		resume(create((void *)xsh_memtest, 4096, 20, "memtest", 2, nargs, args));
	}
	else{
		printf("hello\n");
		printf("list\n");
		printf("prodcons\n");
		printf("prodcons_bb\n");
		printf("futest\n");
		printf("memtest\n");
		return OK;
	}
	return 0;
}