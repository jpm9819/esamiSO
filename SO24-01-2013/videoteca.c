#include "videoteca.h"
#include <stdio.h>

void init_monitor(monitor *m){
  int i, k, n;
  
  pthread_mutex_init(&(m->mutex), NULL);
  k=1;
  n=1;
  for(i=0;i<N_DVD;i++){
    m->dvd[i].stato = DISPONIBILE;
    m->dvd[i].id_copia=k;
    m->dvd[i].id_film=n;
    k = (k % N_COPIE) + 1;
    n = (n % N_ID) + 1;
  }
  for(i=0;i<N_ID;i++)
    pthread_cond_init(&(m->ok_affitta[i]), NULL);
}

void del_monitor(monitor *m){
  int i;
  pthread_mutex_destroy(&(m->mutex));
  for(i=0;i<N_ID;i++)
    pthread_cond_destroy(&(m->ok_affitta[i]));
}

int affitta(monitor *m, int id_film){
  int i, id_copia = -1;
  pthread_mutex_lock (&(m->mutex));
    do{
      for(i=0;i<N_DVD;i++)
        if((m->dvd[i].stato == DISPONIBILE) && (m->dvd[i].id_film==id_film)){
          id_copia = m->dvd[i].id_copia;
          m->dvd[i].stato=AFFITTATO;
          break;
        }
      if(id_copia==-1)
        pthread_cond_wait(&(m->ok_affitta[id_film-1]), &(m->mutex));
    }while(id_copia < 0);
  pthread_mutex_unlock (&(m->mutex));
  return id_copia;
}

void restituisci(monitor *m, int id_film, int id_copia){
  int i;
  pthread_mutex_lock (&(m->mutex));
    for(i=0;i<N_DVD;i++)
      if((m->dvd[i].id_copia==id_copia) && (m->dvd[i].id_film==id_film)){
        m->dvd[i].stato = DISPONIBILE;
      }
  pthread_cond_signal (&(m->ok_affitta[id_film-1]));
  pthread_mutex_unlock (&(m->mutex)); 
}

void stampa(monitor *m){
  int i;
  for(i=0;i<N_DVD;i++)
    printf("[DVD %d]: IDF:%d\tIDC:%d\tSTATO:%s\n", i, m->dvd[i].id_film, m->dvd[i].id_copia, (m->dvd[i].stato==DISPONIBILE) ? "DISPONIBILE" : "AFFITTATO");
}
