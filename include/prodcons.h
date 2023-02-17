/* Global variable for producer consumer */
extern int n; /* this is just declaration */

extern sid32 consumed;
extern sid32 produced;
extern sid32 done;

/* Function Prototype */
void consumer(int count);
void producer(int count);

