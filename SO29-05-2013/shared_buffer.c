#include "shared_buffer.h"
#include <string.h>

void init_shared_buffer (shared_buffer *s){
  s->head = 0;
  s->tail = 0;
  s->n_elem = 0;
  
  pthread_mutex_init (&(s->mutex), NULL);
  pthread_cond_init (&(s->not_full), NULL);
  pthread_cond_init (&(s->not_empty), NULL);
}

void del_shared_buffer (shared_buffer *s){
  pthread_mutex_destroy (&(s->mutex));
  pthread_cond_destroy (&(s->not_full));
  pthread_cond_destroy (&(s->not_empty));
}

void enqueue_sb (shared_buffer *s, m_req *m){
  pthread_mutex_lock (&(s->mutex));
  while(s->n_elem==DIM_SB)
    pthread_cond_wait (&(s->not_full), &(s->mutex));
  memcpy(&(s->buf[s->tail]), m, sizeof(m_req));
  s->tail = (s->tail + 1) % DIM_SB;
  s->n_elem++;
  pthread_cond_signal (&(s->not_empty));
  pthread_mutex_unlock (&(s->mutex));
}

void dequeue_sb (shared_buffer *s, m_req *m){
  pthread_mutex_lock (&(s->mutex));
  while(s->n_elem==0)
    pthread_cond_wait (&(s->not_empty), &(s->mutex));
  memcpy(m, &(s->buf[s->head]), sizeof(m_req));
  s->head = (s->head + 1) % DIM_SB;
  s->n_elem--;
  pthread_cond_signal (&(s->not_full));
  pthread_mutex_unlock (&(s->mutex));
}
