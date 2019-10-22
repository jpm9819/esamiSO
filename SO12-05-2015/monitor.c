#include "monitor.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static int cercalibero(gestioneVoli *g);
static int cercavolo(gestioneVoli*, int);

void init_gestioneVoli(gestioneVoli *g){
  int i;
  g->n_liberi = DIM;
  pthread_mutex_init(&(g->mutex), NULL);
  pthread_cond_init(&(g->produttori), NULL);
  for(i=0;i<DIM;i++)
    g->stato[i] = LIBERO;
}

void del_gestioneVoli(gestioneVoli *g){
  pthread_mutex_destroy(&(g->mutex));
  pthread_cond_destroy(&(g->produttori));
}

void inserisciVolo(gestioneVoli *g, int id_volo){
  int id_index;
  
  pthread_mutex_lock(&(g->mutex));
  while(g->n_liberi==0)
    pthread_cond_wait(&(g->produttori),&(g->mutex));
  
  id_index = cercalibero (g);
  g->stato[id_index] = IN_USO;
  g->n_liberi--;
  
  pthread_mutex_unlock (&(g->mutex));
  
  g->vet_voli[id_index].quota=0;
  g->vet_voli[id_index].id=id_volo;
  sleep(1);
  
  pthread_mutex_lock (&(g->mutex));
  
  g->stato[id_index] = OCCUPATO;
  
  pthread_mutex_unlock(&(g->mutex));
}

void rimuoviVolo(gestioneVoli *g, int id_volo){
  int id_index;
  
  pthread_mutex_lock(&(g->mutex));
  
  if((id_index = cercavolo (g, id_volo))==-1){
    printf("Errore: id_volo: %d non esiste\n", id_volo);
    pthread_mutex_unlock(&(g->mutex));
    return;
  }
  g->stato[id_index]=IN_USO;
  pthread_mutex_unlock(&(g->mutex));
  sleep(1);
  pthread_mutex_lock (&(g->mutex));
  g->n_liberi++;
  g->stato[id_index]=LIBERO;
  pthread_cond_signal (&(g->produttori));
  pthread_mutex_unlock(&(g->mutex));
}

void aggiornaVolo(gestioneVoli *g, int id_volo, int new_quota){
  int id_index;
  
  pthread_mutex_lock (&(g->mutex));
  if((id_index = cercavolo (g, id_volo))==-1){
    printf("Errore: id_volo: %d non esiste\n", id_volo);
    pthread_mutex_unlock(&(g->mutex));
    return;
  }
  g->stato[id_index]=IN_USO;
  pthread_mutex_unlock(&(g->mutex));
  sleep(1);
  sleep(rand() % 3);
  g->vet_voli[id_index].quota=new_quota;
  pthread_mutex_lock (&(g->mutex));
  g->stato[id_index] = OCCUPATO;
  pthread_mutex_unlock(&(g->mutex));
}

int cercalibero(gestioneVoli *g){
  int i;
  for(i=0;i<DIM;i++)
    if(g->stato[i]==LIBERO)
      return i;
  return -1;
}

int cercavolo(gestioneVoli *g, int id_volo){
  int i;
  for(i=0;i<DIM;i++)
    if(g->stato[i]==OCCUPATO)
      if(g->vet_voli[i].id==id_volo)
        return i;
  return -1;
}
