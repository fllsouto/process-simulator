/*
*
*   Fellipe Souto Sampaio - 7990422
*   Computer Science Undergraduate - University of São Paulo (IME/USP)
*   Operational Systems - Program Exercise I - Shell
*   Prof: Daniel M. Batista
*
*   September, 2015
*
*
*   Files:
*
*   -main.c
*   -commandLine.c
*   -coreCommands.c
*   -fcfs_core.c
*   -pr_core.c
*   -rr_core.c
*   -rtshd_core.c
*   -scheduler_core.c
*   -scheduler_main.c
*   -sjf_core.c
*   -srtf_core.c
*   -stderr_msg.c
*
*/

/*
*   Headers
*/

#include <stdio.h>
#include <stdlib.h>
#include "../headers/commandLine.h"

/*
*   Constants
*/


#define EXIT_FLAG 0
#define ERROR_FLAG 0

/*
*   Global Variables
*/

int exit_flag = 0;
int exit_error_flag = 0;

/*
*   Functions Definition
*/


int main(int argc, char* argv[]){
  while(exit_flag == 0){
    run_shel();;
  }
  exit(exit_error_flag);
}