#include "priorityBuffer.h"
#include <stdlib.h>
#include <stdio.h>

void init_priorityBuffer(priorityBuffer *p){
  init_monitor (&(p->m), N_VARCOND);
  p->testa = p->coda = p->n_elem = 0;
}

void del_priorityBuffer(priorityBuffer *p){
  del_monitor (&(p->m));
}

void prod_HP(priorityBuffer *p){
  enter_monitor (&(p->m));
  while(p->n_elem==DIM_BUF)
    wait_condition (&(p->m), OK_HP);
  p->buffer[p->coda] = rand () % 13;
  printf("[PROD_H] ho prodotto %d\n", p->buffer[p->coda]);
  p->coda = (p->coda + 1) % DIM_BUF;
  p->n_elem++;
  signal_condition (&(p->m), OK_PROD);
  leave_monitor (&(p->m));
}

void prod_LP(priorityBuffer *p){
  enter_monitor (&(p->m));
  while((p->n_elem==DIM_BUF) || (queue_condition (&(p->m), OK_HP)))
    wait_condition (&(p->m), OK_LP);
  p->buffer[p->coda] = 13 + rand () % 13;
  printf("[PROD_L] ho prodotto %d\n", p->buffer[p->coda]);
  p->coda = (p->coda + 1) % DIM_BUF;
  p->n_elem++;
  signal_condition (&(p->m), OK_PROD);
  leave_monitor (&(p->m));
}

void consuma(priorityBuffer *p){
  enter_monitor (&(p->m));
  while(p->n_elem==0)
    wait_condition (&(p->m), OK_PROD);
  printf("Ho consumato %d\n", p->buffer[p->testa]);
  p->testa = (p->testa + 1) % DIM_BUF;
  p->n_elem--;
  signal_condition (&(p->m), OK_HP);
  signal_condition (&(p->m), OK_LP);
  leave_monitor (&(p->m));
}
