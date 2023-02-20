extern int arr_q[5];

extern sid32 mutex;
extern sid32 producer_mutex;
extern sid32 consumer_mutex;

extern int head;
extern int tail;

void consumer_bb(int count);
void producer_bb(int count);
void prodcons_bb(int nargs, char *args[]);