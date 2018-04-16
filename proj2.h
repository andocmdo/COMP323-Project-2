/*  Andy Sisinger
    Project 2 - Header file
*/

#define MAX_THREADS 10 /* just an upper limit on the number of threads for sanity sake */
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *producer_thread(void *param);
void *consumer_thread(void *param);
void printBuffer();
