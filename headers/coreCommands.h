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

#include "../headers/commandLine.h"

/*
*   Constants
*/

#define MAX_SIZE 1024
#define TRUE 1
#define FALSE 0
#define PWD_CMD "pwd"
#define CD_CMD "cd"
#define LS_CMD "/bin/ls"
#define EP1_CMD "./ep1"

/*
*   Functions Prototypes
*/

char* current_dir();
void pwd();
void call_cmd_pwd(int);
void call_cmd_line(Cmd_line* );