#ifndef __SHARBUF_H
#define  __SHARBUF_H

#include <pthread.h>
#include "msg.h"

#define DIM_SB 3

typedef struct {
  m_req buf[DIM_SB];
  
  int head;
  int tail;
  int n_elem;
  
  pthread_mutex_t mutex;
  pthread_cond_t not_full;
  pthread_cond_t not_empty;
} shared_buffer;

void init_shared_buffer (shared_buffer*);
void del_shared_buffer (shared_buffer*);

void enqueue_sb(shared_buffer*, m_req*);
void dequeue_sb(shared_buffer*, m_req*);

#endif
