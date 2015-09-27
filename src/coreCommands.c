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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../headers/coreCommands.h"
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
*   Functions Definition
*/

/*******************************************************************************************/
/*
* Essa função cria um novo processo e chama o simulador de processos
*/
pid_t cmd_ep1(char* command_line){
  pid_t child_pid = 0;
  if ((child_pid = fork()) != 0) {
    return child_pid;
  } else {
    char argv1[MAX_SIZE] = "";
    char argv2[MAX_SIZE] = "";
    char argv3[MAX_SIZE] = "";
    char argv4[MAX_SIZE] = "";
    char* new_argv[6];
    char* new_env[1];
    char prog_name[] = EP1_CMD;

    sscanf(command_line, "%*s %s %s %s %s", argv1, argv2, argv3, argv4);
    new_argv[0] = prog_name;
    new_argv[1] = argv1;
    new_argv[2] = argv2;
    new_argv[3] = argv3;
    if (strcmp("", argv4) == 0) {
      new_argv[4] = NULL;
    } else {
      new_argv[4] = argv4;
    }

    new_argv[5] = NULL;
    new_env[0]  = NULL;

    if (execve(prog_name, new_argv, new_env) == -1) {
      perror("Error : ");
      printf("\n");
      exit(2);
    }
  }
  return child_pid;
}

/*
* Interface externa para chamarem o comando ./ep1 <args> 
*/

void call_cmd_ep1(char* command_line){
  char arg[MAX_SIZE] = "";
  pid_t child_pid;

  sscanf(command_line, "%*s %s", arg);
  child_pid = cmd_ep1(command_line);

  waitpid(child_pid, 0, 0);
}


/*******************************************************************************************/

/*
* Essa função cria um novo processo e chama o binário /bin/ls 
*/

pid_t ls(char* arg){
  pid_t child_pid = 0;
  if ((child_pid = fork()) != 0) {
    return child_pid;
  } else {
    char* new_argv[3];
    char* new_env[1];
    char prog_name[] = LS_CMD;

    new_argv[0] = prog_name;
    new_argv[1] = arg;
    new_argv[2] = NULL;
    new_env[0]  = NULL;

    if (execve(prog_name, new_argv, new_env) == -1) {
      perror("Error : ");
      printf("\n");
      exit(2);
    }
  }
  return child_pid;
}

/*
* Interface externa para o comando 
*/

void call_cmd_ls(char* command_line){
  char arg[MAX_SIZE] = "";
  pid_t child_pid;

  sscanf(command_line, "%*s %s", arg);
  child_pid = ls(arg);

  waitpid(child_pid, 0, 0);
}
/*******************************************************************************************/

/*
* Essa função troca de diretório
*/

void cd(char* path){
  if (chdir(path) == -1) {
    perror("Erro! : ");
  }
}

/*
* Interface externa para o comando
*/


void call_cmd_cd(char* command_line){
   char path[MAX_SIZE];
   sscanf(command_line, "%*s %s", path);
   cd(path);
 }

/*******************************************************************************************/

/*
* Essa função retorna o diretório atual em string
*/

char* current_dir(){
  char* path = NULL;
  char* str = NULL;
  char buffer[PATH_MAX+1] = "";
  int ln = 0;

  path = getcwd(NULL, PATH_MAX + 1);
  if (path == NULL) {
    perror("Error! :");
    exit(2);
  } else {
    sprintf(buffer, "[%s] ", path);
    ln = (unsigned)strlen(buffer);
    str = malloc((ln+1)*sizeof(char));
    strncpy(str, buffer, ln);
    str[ln] = '\0';
    free(path);
    return str;
  }
}

/*
* Essa função imprime o diretório atual
*/


void pwd(){
  char* str = NULL;
  str = getcwd(NULL, PATH_MAX + 1);
  if (str == NULL) {
    perror("Error! :");
    exit(2);
  } else {
      printf("%s\n", str);
  }
  free(str);
}

/*******************************************************************************************/

/*
* Essa função chaveia o comando para sua interface
*/


void call_cmd_line(Cmd_line* sh_command){
  
  char* command = sh_command->cmd;
  char* command_line = sh_command->cmd_line;

  if (strcmp(command, PWD_CMD) == 0){
    pwd();
  } else if (strcmp(command, CD_CMD) == 0){
    call_cmd_cd(command_line);
  } else if (strcmp(command, EP1_CMD) == 0){
    call_cmd_ep1(command_line);
  } else if (strcmp(command, LS_CMD) == 0){
    call_cmd_ls(command_line);  
  } else {
    printf("\nUndefined command\n");

  }
}