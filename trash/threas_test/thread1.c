#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void *thread_function();

int main(void){

  pthread_t mythread;

  if(pthread_create(&mythread, NULL, thread_function, NULL) ) {
    printf("Error creating thread!");
    abort();
  }

  if(pthread_join(mythread, NULL) ) {
    printf("Error joining thread!");
    abort();
  }

  exit(0);
}

void *thread_function(){
  int i;
  for(i = 0; i < 20; i++){
    printf("Thread says hi \n");
    sleep(1);
  }
  return NULL;
}