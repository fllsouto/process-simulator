#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int myglobal;
pthread_mutex_t mymutext = PTHREAD_MUTEX_INITIALIZER;

void *thread_function();

int main () {
  pthread_t mythread;
  int i;

  if ( pthread_create(&mythread, NULL, thread_function, NULL) ) {
    printf("Error creating thread");
    abort();
  }

  for (i = 0; i < 20; i++) {
    pthread_mutex_lock(&mymutext);
    myglobal = myglobal + 1;
    pthread_mutex_unlock(&mymutext);
    printf("o");
    fflush(stdout);
    sleep(1);
  }

  if ( pthread_join(mythread, NULL) ) {
    printf("Error joining thread");
    abort(); 
  }

  printf("\nmyglobal equals %d\n",myglobal);
  exit(0);
}

void *thread_function() {
  int i, j;
  for (i = 0; i < 20; i++) {
    pthread_mutex_lock(&mymutext);
    j = myglobal;
    j = j + 1;
    myglobal = j;
    pthread_mutex_unlock(&mymutext);
    printf(".");
    fflush(stdout);
    sleep(1);
  }
  return NULL;
}