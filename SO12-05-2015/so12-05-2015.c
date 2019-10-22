#include "monitor.h"
#include <stdlib.h>
#include <stdio.h>

#define N_THREAD  5 

typedef struct {
  gestioneVoli *g;
  int n;
} param;

void* thread_volo(void*);

int main(){
  pthread_t thread[N_THREAD];
  gestioneVoli g;
  param *par;
  int i;
  
  par=(param*)calloc(N_THREAD,sizeof(param));
  
  init_gestioneVoli(&g);
  
  for(i=0;i<N_THREAD;i++){
    par[i].g = &g;
    par[i].n = i;
    pthread_create(&thread[i], NULL, thread_volo, (void*)&par[i]);
  }
  free(par);
  for(i=0;i<N_THREAD;i++)
    pthread_join(thread[i], NULL);
  
  del_gestioneVoli(&g);
  
  return 0;
}

void* thread_volo(void* ptr){
  gestioneVoli *g=((param*)ptr)->g;
  int id = ((param*)ptr)->n;
  
  printf("[THREAD %d] inserimento volo\n", id);
  inserisciVolo (g, id);
  printf("[THREAD %d] aggiornamento quota volo a 1000\n", id);
  aggiornaVolo (g, id, 1000);
  printf("[THREAD %d] aggiornamento quota volo a 2000\n", id);
  aggiornaVolo (g, id, 2000);
  printf("[THREAD %d] aggiornamento quota volo a 1000\n", id);
  aggiornaVolo (g, id, 1000);
  printf("[THREAD %d] eliminazione volo\n", id);
  rimuoviVolo (g, id);
  
  pthread_exit(NULL);
}
