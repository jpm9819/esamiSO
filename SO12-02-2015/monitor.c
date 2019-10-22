#include "monitor.h"
#include <stdlib.h>
#define DEBUG_H
#ifdef DEBUG_H
#include <stdio.h>
#endif

static int ricerca_libero(Monitor_elaborazioni*);
static int ricerca_minimo(Monitor_elaborazioni*);

void init_monitor(Monitor_elaborazioni *m){
  int i;
  
  pthread_mutex_init (&(m->mutex), NULL);
  pthread_cond_init (&(m->produttori), NULL);
  pthread_cond_init (&(m->consumatori), NULL);
  
  m->n_liberi = MAX_ELAB;
  m->n_occupati = 0;
  
  for(i=0;i<MAX_ELAB;i++)
    m->stato[i]=LIBERO;
}

void del_monitor(Monitor_elaborazioni *m){
  pthread_mutex_destroy(&(m->mutex));
  pthread_cond_destroy(&(m->produttori));
  pthread_cond_destroy (&(m->consumatori));
}

void produci_richiesta(Monitor_elaborazioni *m, buffer *b){
  int id_buffer, i;
#ifdef DEBUG_H
  printf("inizio produzione\n");
#endif
  pthread_mutex_lock(&(m->mutex));
  while(m->n_liberi==0)
    pthread_cond_wait(&(m->produttori), &(m->mutex));

#ifdef DEBUG_H
  printf("condizione scrittura verificata\ncon %d posti liberi\n", m->n_liberi);
#endif

  id_buffer = ricerca_libero (m);
  m->stato[id_buffer] = IN_USO;
  m->n_liberi--;
#ifdef DEBUG_H
  printf("indice array: %d\nstato: %d\nposti liberi ora: %d\n", id_buffer, m->stato[id_buffer], m->n_liberi);
#endif
  pthread_mutex_unlock(&(m->mutex));
  
  m->elaborazioni[id_buffer].totale_operandi = b->totale_operandi;
  for(i=0;i<b->totale_operandi;i++)
    m->elaborazioni[id_buffer].operandi[i] = b->operandi[i];
#ifdef DEBUG_H
  printf("ho prodotto\n\ttotale operandi: %d\n", m->elaborazioni[id_buffer].operandi[i]);
#endif  
  pthread_mutex_lock(&(m->mutex));
  m->stato[id_buffer] = OCCUPATO;
  m->n_occupati++;
#ifdef DEBUG_H
  printf("occupati ora: %d\nstato: %d\n",m->n_occupati, m->stato[id_buffer]);
#endif
  pthread_cond_signal(&(m->consumatori));
  pthread_mutex_unlock (&(m->mutex));
}

int consuma_richiesta(Monitor_elaborazioni *m, buffer *b){
  int id_buffer, i, somma;
  
  pthread_mutex_lock(&(m->mutex));
  while(m->n_occupati==0)
    pthread_cond_wait(&(m->consumatori), &(m->mutex));
  
  id_buffer = ricerca_minimo (m);
  m->stato[id_buffer] = IN_USO;
  m->n_occupati--;
  
  pthread_mutex_unlock (&(m->mutex));
  
  for(i=0, somma=0; i<(b->totale_operandi=m->elaborazioni[id_buffer].totale_operandi); i++)
    somma+=(b->operandi[i]=m->elaborazioni[id_buffer].operandi[i]);
  
  pthread_mutex_lock(&(m->mutex));
  
  m->stato[id_buffer] = LIBERO;
  m->n_liberi++;
  
  pthread_cond_signal (&(m->produttori));
  pthread_mutex_unlock (&(m->mutex));
  
  return somma;
}

int ricerca_libero(Monitor_elaborazioni *m){
  int i;
  
  for(i=0;i<MAX_ELAB;i++)
    if(m->stato[i]==LIBERO)
      return i;
  
  return -1;
}

int ricerca_minimo(Monitor_elaborazioni *m){
  int i, min=4, min_index;
  
  for(i=0; i<MAX_ELAB; i++)
    if(m->stato[i]==OCCUPATO)
      if(m->elaborazioni[i].totale_operandi<min){
        min = m->elaborazioni[i].totale_operandi;
        min_index = i;
      }
  
  return min_index;
}
