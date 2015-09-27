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
*   Headers
*/

#include "stdio.h"
#include "../headers/scheduler_main.h"
#include "../headers/stderr_msg.h"

/*
*   Functions Definition
*/

void notify_new_process_arrived(int line, char* name){
  if(stderr_flag) fprintf(stderr, "O processo <%s> da linha [%d] chegou.\n", name, line);
}

void notify_start_cpu_by_worker(int core_num, char* name){
  if(stderr_flag) fprintf(stderr, "O processo <%s> foi atribuído a CPU [%d].\n", name, core_num);
}

void notify_free_cpu_by_worker(int core_num, char* name){
  if(stderr_flag) fprintf(stderr, "O processo <%s> terminou sua execução e a CPU [%d] foi liberada.\n", name, core_num);
}

void notify_blocked_worker(int core_num, char* name1, char* name2, float dt1, float dt2){
  if(stderr_flag) fprintf(stderr, "O processo <%s> (%f) foi bloqueado e a CPU [%d] foi liberada para o processo [%s] (%f).\n", name1, dt1, core_num, name2, dt2);
}

void notify_new_blocked_worker_on_queue(int core_num, char* name1, float dt1){
  if(stderr_flag) fprintf(stderr, "O processo <%s> (%f) da CPU [%d] foi bloqueado inserido na Queue.\n", name1, dt1, core_num);
}

void notify_context_swap_total(int total){
  if(stderr_flag) fprintf(stderr, "Houve um total de [%d] trocas de contexto.\n",total);
}

void notify_finished_worker_exec_and_write_ouput(char* name, float tf, float tr){
  if(stderr_flag) fprintf(stderr, "O processo <%s> terminou.\nA linha [%s -- %f -- %f] será escrita no arquivo de saída.\n", name, name, tf, tr);
}
