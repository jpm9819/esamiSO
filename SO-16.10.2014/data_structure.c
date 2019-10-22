#include "data_structure.h"
#include <stdio.h>

void init_mon_buf(mon_buf *m){
  m->occupato=0;
  m->nlettori=0;
  pthread_mutex_init (&m->mutex, NULL);
  pthread_cond_init (&m->libero, NULL);
}

void init_mon_vet(mon_vet *m){
  m->count=0;
  m->head=0;
  m->tail=0;
  pthread_mutex_init (&m->mutex, NULL);
  pthread_cond_init (&m->not_empty, NULL);
  pthread_cond_init (&m->not_empty, NULL);
}

void rem_mon_buf(mon_buf *m){
  pthread_mutex_destroy (&m->mutex);
  pthread_cond_destroy (&m->libero);
}

void rem_mon_vet(mon_vet *m){
  pthread_mutex_destroy (&m->mutex);
  pthread_cond_destroy (&m->not_full);
  pthread_cond_destroy (&m->not_empty);
}

void genera(mon_vet *m, elem e){
  pthread_mutex_lock (&m->mutex);
  while(m->count == DIM)
    pthread_cond_wait (&m->not_full, &m->mutex);
  m->vet[m->tail] = e;
  m->tail = (m->tail + 1) % DIM;
  m->count++;
  pthread_cond_signal (&m->not_empty);
  pthread_mutex_unlock (&m->mutex);
}

elem preleva(mon_vet *m){
  elem e;
  pthread_mutex_lock (&m->mutex);
  while(m->count==0)
    pthread_cond_wait (&m->not_empty, &m->mutex);
  e = m->vet[m->head];
  m->head = (m->head + 1) % DIM;
  m->count--;
  pthread_cond_signal (&m->not_full);
  pthread_mutex_unlock (&m->mutex);
  return e;
}

void aggiorna(mon_buf *m, elem e){
  pthread_mutex_lock (&m->mutex);
  m->occupato = 1;
  while(m->nlettori>0)
    pthread_cond_wait (&m->libero, &m->mutex);
  m->buf = e;
  m->occupato = 0;
  pthread_mutex_unlock (&m->mutex);
}

void consulta(mon_buf *m){
  pthread_mutex_lock (&m->mutex);
  m->nlettori++;
  printf("%d + %d = %d\n", m->buf.a, m->buf.b, m->buf.a + m->buf.b);
  m->nlettori--;
  if(m->nlettori==0) pthread_cond_signal(&m->libero);
  pthread_mutex_unlock (&m->mutex);
}
