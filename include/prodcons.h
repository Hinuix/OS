/* Global variable for producer consumer */
extern int n; /* this is just declaration */

sid32 produced;
sid32 consumed;

/* Function Prototype */
void consumer(int count);
void producer(int count);