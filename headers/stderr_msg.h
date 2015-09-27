/*
*
*   Fellipe Souto Sampaio - 7990422
*   Computer Science Undergraduate - University of São Paulo (IME/USP)
*   Operational Systems - Program Exercise I - Process Scheduler
*   Prof: Daniel M. Batista
*
*   September, 2015
*
*/

/*
*   External Variables
*/

#ifndef _stderr_msg_h
#define _stderr_msg_h

extern int stderr_flag;

/*
*   Functions Prototypes
*/

void notify_new_process_arrived(int, char*);
void notify_start_cpu_by_worker(int, char*);
void notify_free_cpu_by_worker(int, char*);
void notify_context_swap_total(int);
void notify_finished_worker_exec_and_write_ouput(char*, float, float);
void notify_blocked_worker(int , char* , char*, float, float);
void notify_new_blocked_worker_on_queue(int , char* , float );
#endif