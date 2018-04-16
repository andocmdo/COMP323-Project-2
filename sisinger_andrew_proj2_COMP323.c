/*  Andy Sisinger
    Project 2 - Process Syncroniztion Problem
    11/09/17
    COMP323
*/

#include <stdio.h>      /* For basic IO */
#include <stdlib.h>     /* For rand() */
#include <pthread.h>    /* For pthreads */
#include <time.h>       /* needed for the time seed */
#include <unistd.h>     /* sleep() / usleep()  */
#include <semaphore.h>  /* for semaphores */
#include "buffer.h"     /* the buffer header file */
#include "proj2.h"      /* this project header file */

/* Globals */
buffer_item buffer[BUFFER_SIZE];  /* a buffer array using size from buffer.h file */
sem_t empty, full;                /* semaphores for full and empty in buffer */
pthread_mutex_t mutex; /* pthreads mutex for locking */
int head, tail, count;   /* variables to keep track of my circular array buffer */


int main (int argc, char *argv[])
{
  srand(time(NULL)); /* makes a new seed for the prng */
  /* 1. Get command line arguments argv[1], argv[2], argv[3]*/
  int sleepyTime, numProd, numCons; /* sleep time, producers, consumers */
  /* get the actual variables */
  if (argc != 4) {
    printf("\nUsage: ./proj2 [sleep in sec] [number of producer threads] [number of consumer threads]\n\n");
    return 1; // exit with error if not the right amount of cmdline args
  }
  sleepyTime = atoi(argv[1]);
  numProd = atoi(argv[2]);
  numCons = atoi(argv[3]);
  /* some sanity checking for the inputs */
  if (sleepyTime < 1 || numProd < 1 || numCons < 1 ) {
    printf("\nSleep time and thread counts must all be positive integers!\n");
    return 2;
  } else if ( numProd > MAX_THREADS || numCons > MAX_THREADS ) {
    printf("\nThat is an unreasonable number of threads! try again!\n");
    return 2;
  }
  printf("\nsleep time=%d  producers=%d  conssumers=%d  \n", sleepyTime, numProd, numCons);


  /* 2. Initialize buffer */
  head = 0; /* front points to the front/first index of the circular array */
  tail = 0; /* points to the back/last index of the circular array */
  count = 0; /* how many spots are occupied */
  for (int i=0; i<BUFFER_SIZE; i++) {
    buffer[i] = -1; /* I used -1 to indicate an empty spot in buffer for ease of reading */
  }

  /* initialize the semaphores and mutex */
  pthread_mutex_init(&mutex, NULL);
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, BUFFER_SIZE);


  /* 3. Create Producer threads */
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  for (int i=0; i<numProd; i++) {
    pthread_t thread;
    pthread_create(&thread, &attr, producer_thread, NULL);
  }


  /* 4. Create Consumer threads */
  for (int i=0; i<numCons; i++) {
    pthread_t thread;
    pthread_create(&thread, &attr, consumer_thread, NULL);
  }


  /* 5. Sleepytime */
  printf("\nMain thread going to sleep for %d seconds...\n", sleepyTime);
  sleep(sleepyTime);
  printf("\nMain thread has awoken from sleep!\n");

  /* 6. Exit */
  printf("\nEND\n");
  return 0;
}

/* insert item into buffer, return 0 if all good, -1 otherwise */
int insert_item(buffer_item item) {
  if (count < BUFFER_SIZE) {
    buffer[head] = item;
    count++;
    if (++head >= BUFFER_SIZE) {
      /* we've gone off the end of the array, now to loop back */
      head = 0;
    }
    printBuffer();
    return 0;
  }
  return -1;
}

/* remove item from buffer and place it in item, return 0 if all ok, -1 otherwise */
int remove_item(buffer_item *item) {

  if (count > 0) {
    *item = buffer[tail];
    buffer[tail] = -1;
    count--;
    if (++tail >= BUFFER_SIZE) {
      /* we've gone off the end of the array, loop back to front */
      tail = 0;
    }
    printBuffer();
    return 0;
  }
  return -1;
}

/* thread that produces buffer items and tries to insert them */
void *producer_thread(void *param) {
  printf("\nproducer created!\n");
  /* infinite loop for producing and adding to buffer */
  buffer_item item;
  do {
    usleep(rand() % 1000000); /* I wanted a shorter interval than 1s, so used usleep() */
    item = rand() % RAND_NUM_HIGH_LIMIT; /* generate a random item (int) */
    sem_wait(&empty); /* wait for an empty spot to open */
    pthread_mutex_lock(&mutex); /* acquire lock */
    if (insert_item(item)) {
      /* if insert item return is not 0, that's an error, so print and error */
      printf("ERROR! insert_item() failed!!!\n");
      /* should we exit here? */
    } else {
      printf("%d was inserted into the buffer \n", item);
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
  } while(1);

  pthread_exit(NULL);
}

void *consumer_thread(void *param) {
  printf("\nconsumer created!\n");
  /* infinite loop for producing and adding to buffer */
  buffer_item item;
  do {
    usleep(rand() % 1000000); /* I wanted a shorter interval than 1s, so used usleep() */
    sem_wait(&full); /* wait for something to consume */
    pthread_mutex_lock(&mutex); /* acquire lock */
    if (remove_item(&item)) {
      /* if remove item return is not 0, that's an error, so print and error */
      printf("ERROR! remove_item() failed!!!\n");
      /* should we exit here? */
    } else {
      printf("%d was removed from the buffer \n", item);
    }
    pthread_mutex_unlock(&mutex); /* release lock */
    sem_post(&empty); /* there is now an empty spot in the buffer */
  } while(1);
  pthread_exit(NULL);
}

/* Just a helper method to print the buffer in a nice way */
void printBuffer() {
  for (int i=0; i<BUFFER_SIZE; i++) {
    if (i<BUFFER_SIZE-1) {
      printf("%d, ", buffer[i]);
    } else {
      printf("%d    \t", buffer[i]);
    }
  }
}
