#include "data_structure.h"
#include <stdlib.h>

#define N_GEN 1
#define N_AGG 1
#define N_CONS 3
#define N_THREAD N_GEN+N_AGG+N_CONS

int main(){
  pthread_t threads[N_THREAD];
  mon_buf buf;
  mon_vet vet;
  mon m;
  int i;
  
  m.buf=&buf;
  m.vet=&vet;
  srand(1);
  init_mon_buf (&buf);
  init_mon_vet (&vet);
  
  for(i=0;i<N_GEN;i++)
    pthread_create (&threads[i], NULL, generatore, (void*)m.buf);
  for(;i<N_GEN+N_AGG;i++)
    pthread_create (&threads[i], NULL, aggiornatore, (void*)&m);
  for(;i<N_THREAD;i++)
    pthread_create (&threads[i], NULL, consultatore, (void*)m.vet);
  
  for(i=0;i<N_THREAD;i++)
    pthread_join (threads[i], NULL);
    
  return 0;
}
