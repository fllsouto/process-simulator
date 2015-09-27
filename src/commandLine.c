/*
*
*   Fellipe Souto Sampaio - 7990422
*   Computer Science Undergraduate - University of São Paulo (IME/USP)
*   Operational Systems - Program Exercise I - Shell
*   Prof: Daniel M. Batista
*
*   September, 2015
*
*/

/*
*   Headers
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../headers/commandLine.h"
#include "../headers/coreCommands.h"

/*
*   Functions Prototypes
*/

char* safe_string_copy(char *);
void clean_cmd_line();
void execute_cmd_line();
void parse_cmd_line();
int read_cmd_line();

void show_cmd_line();
void show_string_ascii_code(char*);

/*
*   Global Variables
*/

Cmd_line* sh_command = NULL;
char* cmd_line = NULL;

/*
*   Functions Definition
*/

/*
* Função para copia segura de string
*/

char* safe_string_copy(char *source){
  
  char *str = NULL; 
  size_t ln = strlen(source);
  
  if (ln > 0) {
    if(source[ln-1] == '\n'){
      str = malloc((ln)*sizeof(char));
      strncpy(str, source, ln);
      str[ln-1] = '\0';
    } else {
      str = malloc((ln+1)*sizeof(char));
      strncpy(str, source, ln);
      str[ln] = '\0';
    }
  }
  return str;
}

/*
* Função para desalocar a memória alocada durante a inserção de um comando
*/

void clean_cmd_line(){

  free(sh_command->cmd);
  free(sh_command->cmd_line);
  free(cmd_line);
  free(sh_command);
  cmd_line = NULL;
  sh_command = NULL;

}

/*
* Função para executar o comando inserido
*/

void execute_cmd_line(){
  if (strcmp(sh_command->cmd, EXIT_CMD) == 0) {
    exit_flag = 1;
  } else {
    call_cmd_line(sh_command);
  }
}

/*
* Função para parsear a linha de comando
*/

void parse_cmd_line(){
  
  Cmd_line* parsed_cmd = NULL;
  char* cmd = NULL;
  size_t ln;
  
  parsed_cmd = malloc(sizeof(Cmd_line));

  ln = strlen(cmd_line);
  cmd = malloc(ln*sizeof(char));

  sscanf(cmd_line, "%s %*s", cmd);

  parsed_cmd->cmd = safe_string_copy(cmd);
  parsed_cmd->cmd_line = safe_string_copy(cmd_line);

  free(cmd);
  sh_command = parsed_cmd;
}

/*
* Função para ler e salvar no histórico a linha inserida
*/

int read_cmd_line(){
  int cmd_ln = 0;
  char* dir = current_dir();
  
  cmd_line = readline(dir);
  cmd_ln = (unsigned)strlen(cmd_line);
  free(dir);
  
  if(cmd_line != NULL && cmd_ln > 0){
    add_history(cmd_line);
    return 1;
  } else {
    return 0;
  }
}

/*
* Função principal da execução do shell
*/

void run_shel(){
  if(read_cmd_line()){
    parse_cmd_line();
    execute_cmd_line();
    clean_cmd_line();
  }
}


/**********************************************************************************************
*
*                                     Funções de Debug
*
**********************************************************************************************/

/*
* Função de debug que imprime a linha de comando
*/

void show_cmd_line(){
  char* cmd = sh_command->cmd;
  char* cmd_line = sh_command->cmd_line;
  printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  printf("show_cmd_line\n");
  printf("cmd : %s\n", cmd);
  printf("size : %d\n", (unsigned)strlen(cmd));
  show_string_ascii_code(cmd);
  printf("\n######################################\n");
  
  printf("cmd line: %s\n", cmd_line);
  printf("size : %d\n", (unsigned)strlen(cmd_line));
  show_string_ascii_code(cmd_line);
  printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");

}

/*
* Função para debug que imprime o ascii code de um caracter e sua posição de memória
*/

void show_string_ascii_code(char* str){
  
  int ln = (unsigned)strlen(str);
  int i = 0;

  printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  printf("show_string_ascii_code\n");
  printf("STR : %s\n", str);
  for(;i <= ln; i++){
    printf("[%d] :: %d  M[ %p ]\n", i, str[i], (void *) &str[i]);
  }
  printf("<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}