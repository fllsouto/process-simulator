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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include "../headers/scheduler_main.h"
#include "../headers/scheduler_core.h"

#include "../headers/fcfs_core.h"
#include "../headers/sjf_core.h"
#include "../headers/srtf_core.h"
#include "../headers/rr_core.h"
#include "../headers/pr_core.h"
#include "../headers/rtshd_core.h"

/*
*   Constants
*/

#define MAX_SIZE 1024

/*
*   Global Variables
*/

int scheduler_opt = 0;
char* input_file  = NULL;
char* output_file = NULL;
int stderr_flag;
Process* proc_list;
int proc_qnt = 0;

/*
*   Functions Prototypes
*/

void get_arg(int ,char**);
int count_file_lines();
void read_file();
Process create_new_proc(double , char* , double , double , int , int, boolean, Step, Process*, int);
void free_proc_list();
void show_proc_list();
char* safe_string_copy(char *);
void init_semaphores_and_mutex();
void call_scheduler();
void write_output_file();
void describe_file();

/*
*   Functions Definition
*/

int main(int argc, char* argv[]){
  get_arg(argc, argv);
  read_file();
  describe_file();
  call_scheduler();
  /*show_proc_list();*/
  write_output_file();
  free_proc_list();
  printf("\n");
  return 0;
}

void read_file(){
  FILE* file = fopen(input_file, "r");
  double t0 = 0;
  double dt = 0;
  double deadline = 0;
  int priority = 0;
  char name[MAX_SIZE] = "";
  int i = 0;
  char line[MAX_SIZE] = "";

  proc_qnt = count_file_lines();
  if (file == NULL) {
    perror(input_file);
    exit(-1);
  }

  proc_list = malloc((proc_qnt+1)*sizeof(Process));
  while((fgets(line, MAX_SIZE, file) != NULL)){
    if(strcmp("\n", line) != 0){
      sscanf(line, "%lf %s %lf %lf %d", &t0, name, &dt, &deadline, &priority);
      proc_list[i] = create_new_proc(t0, name, dt, deadline, priority, i, FALSE, WAIT, NULL, 0);
      i++;
    }
  }
  
  proc_list[i] = create_new_proc(0, "master", 0, 0, 0, i, TRUE, READY, proc_list, proc_qnt);
  init_semaphores_and_mutex();
  fclose(file);
}

void describe_file() {
  printf("\n############################################################\n");
  printf("Arquivo de trace : %s\n", input_file);
  printf("Arquivo de saída %s\n", output_file);
  switch(scheduler_opt) {
    case FCFS:
      printf("Algoritmo 'First Come First Server'\n");
    break;
    case SJF:
      printf("Algoritmo 'Shortes Job First'\n");
    break;
    case SRTF:
    break;
    case RR:
    break;
    case PS:
    break;
    case RTSHD:
    break;
  }
  if(stderr_flag){
    printf("Flag STDERR : TRUE\n");
  } else {
    printf("Flag STDERR : FALSE\n");
  }
  printf("############################################################\n\n");
}

void init_semaphores_and_mutex(){
  int i;
  for(i = 0; i <= proc_qnt; i++) {
    if(sem_init(&proc_list[i].milestone, 0, 0)) {
      printf("Error while creating semaphores");
      /*free_proc_list();*/
      exit(-1);
    }

    if(pthread_mutex_init(&proc_list[i].mutex_lock, 0)) {
      printf("Error while creating mutex");
      /*free_proc_list();*/
      exit(-1);
    }
  }
}

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

int count_file_lines(){
  FILE* file = fopen(input_file, "r");
  int counter = 0;
  char line[MAX_SIZE] = "";
  if (file == NULL) {
    perror(strcat("Error while reading input file  ", input_file));
    exit(-1);
  }

  while((fgets(line, MAX_SIZE, file) != NULL)){
    if(strcmp("\n", line) != 0){
      counter++;
    }
  }
  fclose(file);
  return counter;
}

void get_arg(int argc,char *argv[]){
  if(argc <= 3 || argc > 5){
      printf ("Formato errado!\n");
      printf ("%s <numero-do-escalonador> <nome-do-arquivo-de-trace> <nome-arquivo-de-saida> [OPCAO]\n", argv[0]);
      printf ("-d :: Exibir eventos na saída de erro(stderr)\n");
      exit (EXIT_FAILURE);
  }
  scheduler_opt = atoi(argv[1]);
  input_file = argv[2];
  output_file = argv[3];
  if (argc == 5 && strcmp("-d", argv[4]) == 0) {
    stderr_flag = TRUE;
  } else {
    stderr_flag = FALSE;
  }

}

Process  create_new_proc(double t0, char* name, double dt, double deadline, int priority, int identifier, boolean master, Step step, Process* list, int list_size){
  Process new_proc;
  new_proc.id            = identifier;
  new_proc.master        = master;
  new_proc.t0            = t0;
  new_proc.dt            = dt;
  new_proc.deadline      = deadline;
  new_proc.priority      = priority;
  new_proc.list          = list;
  new_proc.list_size     = list_size;
  new_proc.step          = step;
  new_proc.name          = safe_string_copy(name);
  return new_proc;
}

void free_proc_list(){
  int i;
  if(proc_qnt != 0){
    for(i = 0; i <= proc_qnt; i++){
      sem_destroy(&proc_list[i].milestone);
      pthread_mutex_destroy(&proc_list[i].mutex_lock);
      free(proc_list[i].name);
    }
    free(proc_list);
  }
}


void show_proc_list(){
 int i = 0;
  if(proc_qnt != 0){
    printf("Total process : %d\n", proc_qnt);
    for(;i < proc_qnt; i++){
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
      printf("t0 : %f\n", proc_list[i].t0);
      printf("name : %s\n", proc_list[i].name);
      printf("dt : %f\n", proc_list[i].dt);
      printf("deadline : %f\n", proc_list[i].deadline);
      printf("priority : %d\n", proc_list[i].priority);
      printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    }
  } 
}

void call_scheduler() {
  switch(scheduler_opt) {
    case FCFS:
      printf("Executando algoritmo 'First Come First Server' em %d processos\n", proc_qnt);
      first_come_first_server(proc_list, proc_qnt);
    break;
    case SJF:
      printf("Executando algoritmo 'Shortes Job First' em %d processos\n", proc_qnt);
      shortest_job_first(proc_list, proc_qnt);
    break;
    case SRTF:
      printf("Executando algoritmo 'Shortes Remaining Time First' em %d processos\n", proc_qnt);
      shortest_remaining_time_first(proc_list, proc_qnt);
    break;
    case RR:
      printf("Executando algoritmo 'Round Robin' em %d processos\n", proc_qnt);
      round_robin(proc_list, proc_qnt);
    break;
    case PS:
      printf("Executando algoritmo 'Prioriy Scheduling' em %d processos\n", proc_qnt);
      priority_scheduling(proc_list, proc_qnt);
    break;
    case RTSHD:
      printf("Executando algoritmo 'Realtime Scheduling with Hard Deadline' em %d processos\n", proc_qnt);
      realtime_scheduling_with_hard_deadline(proc_list, proc_qnt);
    break;
  }
}

void write_output_file(){
  int i;
  FILE* file = fopen(output_file, "w");
  if (file == NULL) {
    perror(strcat("Error while writing input file  ", output_file));
    exit(-1);
  }
  for(i = 0; i < proc_qnt; i++){
    fprintf(file, "%s %f %f\n", proc_list[i].name, proc_list[i].time_elapsed, proc_list[i].time_elapsed - proc_list[i].t0);
  }
  fprintf(file, "Mudanças de contexto : %d\n", proc_list[i].context_swap);
  fclose(file);
  printf("Fim da execução, saída gravada em : %s\n", output_file);
}