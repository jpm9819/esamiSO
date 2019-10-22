#include "videoteca.h"
#include <stdio.h>
#include <unistd.h>

#define N_THREAD 4

void* thread(void*);
void* t_stampa(void*);

int main(){
  monitor m;
  int i;
  pthread_t threads[N_THREAD+1];
  pthread_attr_t attr;
  
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
  init_monitor (&m);
  
  printf("[MAIN] Sto inizializzando i thread\n");
  for(i=0;i<N_THREAD;i++)
    pthread_create(&threads[i], NULL, thread, (void*)&m);
  pthread_create(&threads[i], &attr, t_stampa, (void*)&m);
  pthread_attr_destroy (&attr);
  printf("[MAIN] Sto attendendo la fine delle operazioni\n");
  for(i=0;i<N_THREAD;i++)
    pthread_join(threads[i], NULL);
  pthread_cancel (threads[i]);
  printf("[MAIN] Fine\n");
  del_monitor (&m);
  return 0;
}

void* thread(void *arg){
  monitor *m = (monitor*)arg;
  int i;
  int id;
  for(i=1;i<4;i++){
    id = affitta (m, i);
    sleep(1);
    restituisci (m, i, id);
  }
  pthread_exit(NULL);
}

void* t_stampa(void *arg){
  monitor *m = (monitor*)arg;
  while(1){
    pthread_testcancel ();
    stampa (m);
    sleep(1);
  }
}
