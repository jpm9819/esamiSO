#ifndef __SHARED_MEM_H
#define  __SHARED_MEM_H

#include "../hoare/monitor.h"
#include <sys/types.h>

#define DIM 10
#define NOT_FULL 0
#define NOT_EMPTY 1
#define N_VARCOND 2

typedef struct {
  unsigned int posizione;
  pid_t pid;
} richiesta;

typedef struct {
  richiesta buf[DIM];
  
  int head;
  int tail;
  int dim;
  
  monitor m;
} shared_queue;

void init_shared_queue(shared_queue*);
void del_shared_queue(shared_queue*);

void enqueue_sq(shared_queue*, richiesta*);
void dequeue_sq(shared_queue*, richiesta*);

#endif
