#include "stack.h"
#include <stdlib.h>

void init_stack(stack *s, int dim){
  s->head = 0;
  s->dim = dim;
  s->dati = (elem*)calloc(dim,sizeof(elem));
  pthread_mutex_init(&(s->mutex), NULL);
  pthread_cond_init(&(s->full), NULL);
  pthread_cond_init(&(s->empty), NULL);
}

void del_stack(stack *s){
  free(s->dati);
  pthread_mutex_destroy(&(s->mutex));
  pthread_cond_destroy(&(s->full));
  pthread_cond_destroy(&(s->empty));
}

void push_s(stack *s, elem e){
  pthread_mutex_lock(&(s->mutex));
  while(s->head==s->dim)
    pthread_cond_wait(&(s->full), &(s->mutex));
  s->dati[s->head++] = e;
  pthread_cond_signal(&(s->empty));
  pthread_mutex_unlock(&(s->mutex));
}

elem pop_s(stack *s){
  elem e;
  pthread_mutex_lock(&(s->mutex));
  while(s->head==0)
    pthread_cond_wait(&(s->empty),&(s->mutex));
  e = s->dati[--s->head];
  pthread_cond_signal(&(s->full));
  pthread_mutex_unlock(&(s->mutex));
  return e;
}

int get_size(stack *s){
  int out;
  pthread_mutex_lock(&(s->mutex));
  out = s->head;
  pthread_mutex_unlock(&(s->mutex));
  return out;
}
