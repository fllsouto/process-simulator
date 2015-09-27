#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_PATTERN " "
#define MAX_SIZE 256

// char* read_line(char *str_foo){
//   printf("str_foo ant add : %p\n",(void*)str_foo);
//   char str[MAX_SIZE] = "";
//   if(fgets(str, MAX_SIZE, stdin) != NULL){
//     size_t ln = strlen(str);
//     if(ln > 0 && str[ln-1] == '\n'){ str[--ln] = '\0'; }
//     str_foo = malloc(ln*sizeof(char));
//     strcpy(str_foo, str);
//     printf("str : %s\n", str);
//     printf("str len : %d\n", (unsigned) strlen(str));
//     printf("str_foo : %s\n", str_foo);
//     printf("str_foo len : %d\n", (unsigned) strlen(str_foo));
//     printf("str_foo dp add : %p\n",(void*)str_foo);
//     return str_foo;
//   }else{
//     printf("Error");
//     exit(EXIT_FAILURE);
//   } 
// }

char* read_line(){
  char str[MAX_SIZE] = "";
  char *read_str= NULL;
  if(fgets(str, MAX_SIZE, stdin) != NULL){
    size_t ln = strlen(str);
    read_str = malloc(ln*sizeof(char));
    if(ln > 0 && str[ln-1] == '\n'){ str[--ln] = '\0'; }
    strcpy(read_str, str);
    return read_str;
  }else{
    printf("Error");
    exit(EXIT_FAILURE);
  } 
}

void tokenize_line(char* string){
  char* aux;
  char* aux2;
  int counter = 0;
  aux = strtok(string, TOKEN_PATTERN);
  aux2 = strtok(NULL, TOKEN_PATTERN);

  printf("String antes : %s\n", string);
  printf("[1] : %s\n", aux);
  printf("[2] : %s\n", aux2);
}

void teste_sscanf(char* string){
  char st[100];
  char st2[100];
  printf("String inserida : %s\n", string);
  sscanf(string, "%s %*s", st);
  sscanf(string, "%*s %s", st2);
  printf("string 1 : %s\n",st);
  printf("string 2 : %s\n",st2);
}

int main(){
  char *str = NULL;
  str = read_line();
  printf("Line : %s\n",str);
  // tokenize_line(str);
  teste_sscanf(str);
  free(str);
  return 0;
}