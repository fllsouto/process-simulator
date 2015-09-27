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

#ifndef _scheduler_core_h
#define _scheduler_core_h

/*
*   Constants
*/

#define WAIT      0
#define READY     1
#define RUNNING   2
#define BLOCKED   3
#define PROCESSED 4
#define FAILED    5

#define NOT_FREE_CORE -1
#define TRUE 1
#define FALSE 0
#define SEC_TO_NANO 1000000000

/*
*   Data Structures
*/

typedef int boolean;
typedef int Step;
typedef struct process {
  double t0;
  char* name;
  double dt;
  double deadline;
  int priority;
  int id;
  boolean master;
  boolean quantum_break;
  Step step;
  struct process* list;
  int list_size;
  int context_swap;
  float time_elapsed;
  float cpu_elapsed;
  sem_t milestone;
  pthread_mutex_t mutex_lock;
} Process;

typedef struct core {
  boolean busy;
  Process* working_proc;
  int id;
} Core;

typedef struct queue
{
  Process* proc;
  struct queue* next;
} Process_queue;

/*
*   Global Variables
*/


struct timespec start_time;
struct timespec end_time;
clock_t start_cpu;
clock_t end_cpu;
int processed_threads;


#endif

/*
*   Functions Prototypes
*/


Process_queue* init_queue();
Process_queue* insert_on_queue(Process*, Process_queue*);
Process* remove_from_queue(Process_queue* );
void free_queue(Process_queue*);
void print_queue(Process_queue*);
void print_as_a_queue(Process_queue*);
boolean queue_is_empty(Process_queue*);