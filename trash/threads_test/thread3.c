#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int main(){
  int cores = sysconf(_SC_NPROCESSORS_CONF);
  printf("Total cores is : %d\n", cores);
  printf("\n");
  return 0;
}