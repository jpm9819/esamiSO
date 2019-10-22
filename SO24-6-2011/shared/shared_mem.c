#include "shared_mem.h"
#include <string.h>

void init_shared_queue(shared_queue *s){
  init_monitor (&(s->m), N_VARCOND);
  s->head = 0;
  s->tail = 0;
  s->dim = 0;
}

void del_shared_queue(shared_queue *s){
  del_monitor (&(s->m));
}

void enqueue_sq(shared_queue *s, richiesta *ric){
  enter_monitor (&(s->m));
  while(s->dim==DIM)
    wait_condition (&(s->m), NOT_FULL);
  memcpy(&(s->buf[s->tail]), ric, sizeof(richiesta));
  s->tail = (s->tail + 1) % DIM;
  s->dim++;
  signal_condition (&(s->m), NOT_EMPTY);
  leave_monitor (&(s->m));
}

void dequeue_sq(shared_queue *s, richiesta *ric){
  enter_monitor (&(s->m));
  while(s->dim==0)
    wait_condition (&(s->m), NOT_EMPTY);
  memcpy(ric, &(s->buf[s->head]), sizeof(richiesta));
  s->head = (s->head + 1) % DIM;
  s->dim--;
  signal_condition (&(s->m), NOT_FULL);
  leave_monitor (&(s->m));
}
