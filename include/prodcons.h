/* Global variable for producer consumer */
extern int n; /* this is just declaration */

/* Function Prototype */
void producer(int count, sid32 prod_sem, sid32 cons_sem);
void consumer(int count, sid32 prod_sem, sid32 cons_sem);


extern int arr_q[];
extern int arr_tail;
extern int arr_head;
extern sid32 mutex_bb;

/* Function Prototype */
void producer_bb(int idx, int count, sid32 prod_sem, sid32 cons_sem);
void consumer_bb(int idx, int count, sid32 prod_sem, sid32 cons_sem);

bool isdigit(char c);
bool is_integer_string(char* str);