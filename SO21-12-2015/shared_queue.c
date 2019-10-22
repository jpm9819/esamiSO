#include "shared_queue.h"
#include <string.h>

void init_gestioneIO(gestioneIO *g){
  g->dim = 0;
  g->head = 0;
  g->tail = 0;
  pthread_mutex_init (&(g->mutex), NULL);
  pthread_cond_init (&(g->not_full), NULL);
}

void del_gestioneIO(gestioneIO *g){
  pthread_mutex_destroy (&(g->mutex));
  pthread_cond_destroy (&(g->not_full));
}

void produci(gestioneIO *g, buffer* b){
  pthread_mutex_lock (&(g->mutex));
  while(g->dim == DIM_IO)
    pthread_cond_wait (&(g->not_full), &(g->mutex));
  g->dim++;
  memcpy(&(g->buf[g->tail]), b, sizeof(buffer));
  g->tail = (g->tail + 1) % DIM_IO;
  pthread_mutex_unlock (&(g->mutex));
}

int consuma(gestioneIO *g, buffer* b){
  pthread_mutex_lock (&(g->mutex));
  if(g->dim == 0){
    pthread_mutex_unlock (&(g->mutex));
    return 1;
  }
  g->dim--;
  memcpy(b, &(g->buf[g->head]), sizeof(buffer));
  g->head = (g->head + 1) % DIM_IO;
  pthread_cond_signal (&(g->not_full));
  pthread_mutex_unlock (&(g->mutex));
  return 0;
}
