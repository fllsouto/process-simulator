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

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "../headers/scheduler_core.h"

/*
*   Functions Definition
*/

Process_queue* init_queue() {
  Process_queue* proc_q;
  proc_q = malloc(sizeof(proc_q));
  proc_q->next = proc_q;
  proc_q->proc = NULL;
  return proc_q;
}

boolean queue_is_empty(Process_queue* head) {
  if(head == head->next) {
    return TRUE;
  } else if (head == NULL){
    return -1;
  } else {
    return FALSE;
  }
}

Process_queue* insert_on_queue(Process* proc, Process_queue* head) {
  Process_queue* new;
  if(head != NULL){ 
    new         = malloc(sizeof(Process_queue));
    new->next   = head->next;
    new->proc    = head->proc;
    head->proc   = proc;
    head->next  = new;
    return new;
  } else {
    return NULL;
  }
}

Process* remove_from_queue(Process_queue* head) {
  Process* proc;
  Process_queue* aux;
  if(head != NULL) {
    aux = head->next;
    if(&head == &aux) {
      return NULL;
    } else {
      proc = aux->proc;
      head->next = aux->next;
      free(aux);
      return proc;
    }
  } else {
    return NULL;
  }
}

void print_queue(Process_queue* head) {
  Process_queue* aux;
  if(head != NULL){
    int i = 0;
    aux = head->next;
    if(head == aux){
      printf("Empty Queue!\n");
    } else{
      while(aux != head){
          printf("[%d] :: [%s]\n",i, aux->proc->name);
          aux = aux->next;
          i++;
      }
    }
  } 
}

void free_queue(Process_queue* head) {
  Process_queue* aux;
  if(head != NULL){
    for(;;){
        aux = head->next;
        if(aux == head) break;
        head->next = aux->next;
        free(aux);
    } 
    free(head);
  }
}

void print_as_a_queue(Process_queue* head) {
  Process_queue* aux;
  if(head != NULL){
    aux = head->next;
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");
    if(head == aux){
      printf("Empty Queue!\n");
    } else{
      while(aux != head){
          printf("[%s] ", aux->proc->name);
          aux = aux->next;
      }
    }
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------\n");
  }
}