#ifndef __PBUFFER_H
#define  __PBUFFER_H

#include "../monitor/monitor.h"

#define DIM_BUF 3

#define OK_HP 0
#define OK_LP 1
#define OK_PROD 2
#define N_VARCOND 3

typedef struct {
  int buffer[DIM_BUF];

  int n_elem;
  
  int testa;
  int coda;
  
  monitor m;
} priorityBuffer;

void init_priorityBuffer(priorityBuffer*);
void del_priorityBuffer(priorityBuffer*);

void prod_HP(priorityBuffer*);
void prod_LP(priorityBuffer*);
void consuma(priorityBuffer*);

#endif
