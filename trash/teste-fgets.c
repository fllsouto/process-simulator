#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 128

char* read_line_getline(){
  char* str;
  size_t len = 0;
  if(getline(&str, &len, stdin) != -1){
    size_t ln = strlen(str);
    if(ln > 0 && str[ln-1] == '\n'){ str[--ln] = '\0'; }
    return str;
  }else{
    printf("Error");
    exit(EXIT_FAILURE);
  }
}

char* read_line_fgets(){
  char* str;
  size_t len = 0;
  if(fgets(str, MAX_SIZE, stdin) != NULL){
    size_t ln = strlen(str);
    if(ln > 0 && str[ln-1] == '\n'){ str[--ln] = '\0'; }
    return str;
  }else{
    printf("Error");
    exit(EXIT_FAILURE);
  } 
}

int main(){
  char* str;
  printf("Lendo string : ");
  str = read_line_getline();
  printf("String lida : %s\n", str);
  printf("Tamanho : %d\n", (unsigned) strlen(str));
  printf("---------------\n");
  printf("Lendo string : ");
  free(str);
  str = read_line_fgets();
  printf("String lida : %s\n", str);
  printf("Tamanho : %d\n", (unsigned) strlen(str));
  printf("---------------");
  return 0;
}