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
*   Data Structures and Typedefs
*/

#ifndef _commandLine_h
#define _commandLine_h

struct Command {
  char* cmd;
  char *cmd_line;
};

typedef struct Command Cmd_line; 

/*
*   Functions Prototypes
*/

void run_shel();

/*
*   External Variables
*/

extern int exit_flag;
extern int exit_error_flag;

/*
*   Constants
*/

#define EXIT_CMD "exit"
#define TOKEN_PATTERN  " "
#define MAX_SIZE 1024
#define TRUE 1
#define FALSE 0
/*
*   Global Variables
*/

Cmd_line* sh_command;
char* cmd_line;

#endif