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

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <float.h>
#include "../headers/scheduler_main.h"
#include "../headers/scheduler_core.h"
#include "../headers/sjf_core.h"
#include "../headers/stderr_msg.h"

/*
*   Functions Prototypes
*/

static void* do_master_routine(void *);
static Core* create_core_vector(unsigned int );
static void fetch_ready_process(Process* );
static Process* schedule_next_process(Process*, int);
static int get_free_core(Core* , unsigned int);
static void assign_free_core(Process*, Core*);
static void finish_process_and_free_core(Core* , unsigned int);


static void* do_worker_routine(void *);
static void do_thread_job(Process*);

static void calculate_time_elapsed(Process* , struct timespec *, struct timespec *);

/*
*   Functions Definition
*/


void shortest_job_first(Process* proc_list, int proc_qnt) {

  int i;
  pthread_t* thread_array;
  thread_array = malloc((proc_qnt+1)*sizeof(thread_array));
  for(i = 0; i <= proc_qnt; i++) {
    if(proc_list[i].master){
      if(pthread_create(&thread_array[i], NULL, do_master_routine, &proc_list[i]) != 0) {
        printf("Erro while creating master thread\n");
        exit(-1);
      }
    } else {
      if( pthread_create(&thread_array[i], NULL, do_worker_routine, &proc_list[i]) != 0) {
        printf("Erro while creating workers threads\n");
        exit(-1);
      }
    }
  }

  for(i = 0; i <= proc_qnt; i++){
    if( pthread_join(thread_array[i], NULL) ) {
      printf("Erro while joining threads\n");
      exit(-1);
    }
  }
  free(thread_array);

}

static void* do_master_routine(void* args) {
  
  Process* proc = ((Process*) args);
  Core* core_vector;
  int free_core = -1;
  unsigned int internal_cores = sysconf(_SC_NPROCESSORS_CONF);
  core_vector = create_core_vector(internal_cores);
  proc->context_swap = 0;
  processed_threads = 0;
  clock_gettime(CLOCK_REALTIME, &start_time);
  start_cpu = clock();
  
  while(processed_threads < proc->list_size){
    Process* selected_p;
    fetch_ready_process(proc);
    selected_p = schedule_next_process(proc->list, proc->list_size);
    free_core = get_free_core(core_vector, internal_cores);
    if(selected_p != NULL && free_core != NOT_FREE_CORE) assign_free_core(selected_p, &core_vector[free_core]);
    finish_process_and_free_core(core_vector, internal_cores);
  }
  
  end_cpu = (clock() - start_cpu);
  clock_gettime(CLOCK_REALTIME, &end_time);
  calculate_time_elapsed(proc, &end_time, &start_time);
  
  free(core_vector);
  core_vector = NULL;
  
  notify_context_swap_total(proc->context_swap);
  return NULL;
}

static void* do_worker_routine(void* args) {

  Process* proc = ((Process*) args);

  sem_wait(&proc->milestone);
  sem_wait(&proc->milestone);

  do_thread_job(proc);

  pthread_mutex_lock(&proc->mutex_lock);
  if(proc->time_elapsed <= proc->deadline){
    proc->step = PROCESSED;
  } else {
    proc->step = FAILED;
  }
  pthread_mutex_unlock(&proc->mutex_lock);

  notify_finished_worker_exec_and_write_ouput(proc->name, proc->time_elapsed, proc->time_elapsed - proc->t0);
  return NULL;
}

static Core* create_core_vector(unsigned int internal_cores) {
  
  int i;
  Core* core_vector = NULL;

  core_vector = malloc(internal_cores*sizeof(Core));
  
  for(i = 0; i < internal_cores; i++) {
    core_vector[i].busy = FALSE;
    core_vector[i].working_proc = NULL;
    core_vector[i].id = i;
  }
  return core_vector;

}


static void fetch_ready_process(Process* master) {
  int i;
  Process* aux;
  struct timespec current_time;
  float absolute_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  current_time.tv_sec = current_time.tv_sec - start_time.tv_sec;
  if(current_time.tv_nsec > start_time.tv_nsec){
    current_time.tv_nsec = current_time.tv_nsec - start_time.tv_nsec;
  } else {
    current_time.tv_nsec = current_time.tv_nsec - start_time.tv_nsec;
  }
  absolute_time = current_time.tv_sec + (current_time.tv_nsec/ SEC_TO_NANO);

  for(i = 0; i < master->list_size; i++){
    aux = &master->list[i];
    if(absolute_time >= aux->t0 && aux->step == WAIT){
      notify_new_process_arrived(aux->id + 1, aux->name);
      sem_post(&aux->milestone);
      aux->step = READY;
    }
  }
}


static Process* schedule_next_process(Process* proc_list, int list_size) {
  
  int i;
  int indx = -1;
  float dt_min = FLT_MAX;
  for(i = 0; i < list_size; i++) {
    if(proc_list[i].step == READY && proc_list[i].dt < dt_min) {
      indx = i;
      dt_min = proc_list[i].dt;
    }
  }
  if(indx == -1){
    return NULL;
  } else {
    return &proc_list[indx];  
  }

}

static int get_free_core(Core* core_vector, unsigned int internal_cores) {
  
  int i;
  for(i = 0; i < internal_cores; i++){
    if(!core_vector[i].busy) return i;
  }
  return NOT_FREE_CORE;

}
static void assign_free_core(Process* proc, Core* core) {

  pthread_mutex_lock(&proc->mutex_lock);
  proc->step = RUNNING;
  pthread_mutex_unlock(&proc->mutex_lock);
  
  core->busy = TRUE;
  core->working_proc = proc;
  sem_post(&proc->milestone);
  notify_start_cpu_by_worker(core->id, proc->name);

}

static void finish_process_and_free_core(Core* core_vector, unsigned int internal_cores) {

  int i;
  for(i = 0; i < internal_cores; i++) {
    if(core_vector[i].busy && (core_vector[i].working_proc->step == PROCESSED || core_vector[i].working_proc->step == FAILED)) {
      notify_free_cpu_by_worker(core_vector[i].id, core_vector[i].working_proc->name);
      core_vector[i].busy = FALSE;
      core_vector[i].working_proc = NULL;
      processed_threads++;
    }
  }

}

static void do_thread_job(Process* proc) {

  struct timespec begging, end;
  long int seed = 1;
  long int sum = seed;
  float dec = 0.05;
  float limit = dec * SEC_TO_NANO;

  clock_gettime(CLOCK_REALTIME, &begging);
  while(proc->dt > 0) {
    clock_gettime(CLOCK_REALTIME, &end);
    
    if(sum == 1){
      seed++;
      sum = seed;
    } else if(seed >= LONG_MAX/2 ) {
      seed = 1;
      sum = seed;
    } else if(sum % 2 == 0){
      sum = sum/2;
    } else {
      sum = 3*sum + 1;
    }

    if((abs(end.tv_nsec - begging.tv_nsec)) > (limit) ) {
      proc->dt = proc->dt - dec;
      clock_gettime(CLOCK_REALTIME, &begging);
    }
  }

  clock_gettime(CLOCK_REALTIME, &end);
  calculate_time_elapsed(proc, &end, &start_time);

}

static void calculate_time_elapsed(Process* proc, struct timespec* end, struct timespec* start) {

  proc->cpu_elapsed = ((float)(clock() - start_cpu)) / CLOCKS_PER_SEC;
  end->tv_sec = end->tv_sec - start->tv_sec;

  if(end->tv_nsec > start->tv_nsec){
    end->tv_nsec = end->tv_nsec - start->tv_nsec;
  } else {
    end->tv_nsec = end->tv_nsec - start->tv_nsec;
  }
  proc->time_elapsed = end->tv_sec + (end->tv_nsec/ SEC_TO_NANO);

}