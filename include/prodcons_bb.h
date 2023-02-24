extern int32 arr_q[5];

sid32 produced;
sid32 consumed;
sid32 mutex;

extern int32 head;
extern int32 tail;

void consumer_bb(int id, int count);
void producer_bb(int id, int count);