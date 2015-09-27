#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define out_of_memory(x) {  printf("Memoria insuficiente, programa abortado!\n"); \
                         exit(x); \
                      }
/*
*
*   Based on Sedgewirk Quicksort implementation 
*   http://www.ime.usp.br/~pf/algoritmos/aulas/quick.html 
*
*/

void call_quick_sort (int v[], int p, int r) {
   int c, i, j, t;
   if (p < r) {      
      c = v[(p+r)/2];
      i = p, j = r;
      while (i <= j) {
         while (v[i] < c) ++i;
         while (c < v[j]) --j;
         if (i <= j) {
            t = v[i], v[i] = v[j], v[j] = t;
            ++i, --j;
         }
      }
      call_quick_sort (v, p, j);
      call_quick_sort (v, i, r);
   }
}

void print_array(int v[], int ln){
  int i = 0;

  for(; i < ln; i++){
    printf("[%d] :: %d\n", i, v[i]);
  }
}

void populate_random_array(int v[], int ln){
  int i = 0;
  srand(time(NULL));
  for(;i < ln; i++){
    v[i] = rand();
  }
}

void work(int ln){
  int* random_array = NULL;
  clock_t begin, end;
  double time_spent = 0;

  begin = clock();
  
  random_array = malloc(ln*sizeof(int));
  if (!random_array) { out_of_memory(-1); }
  
  populate_random_array(random_array, ln);
  call_quick_sort(random_array, 0, ln-1);
  free(random_array);
  
  end = clock();
  
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time spent : %f\n", time_spent);

}