#include "shared_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N_PROD 4
#define N_CONS 2
#define N_THREAD N_PROD+N_CONS

void* produttore(void*);
void* consumatore(void*);

int main(){
  pthread_t threads[N_THREAD];
  gestioneIO g;
  int i;
  
  srand(time(NULL));
  
  init_gestioneIO (&g);
  
  printf("[MAIN] Sto inizializzando i produttori\n");
  for(i=0;i<N_PROD;i++)
    pthread_create(&threads[i], NULL, produttore, (void*)&g);
  printf("[MAIN] Sto inizializzando i consumatori\n");
  for(;i<N_THREAD;i++)
    pthread_create (&threads[i], NULL, consumatore, (void*)&g);
  
  printf("[MAIN] Sto attendendo la fine delle operazioni\n");
  for(i=0;i<N_THREAD;i++)
    pthread_join(threads[i], NULL);
  
  printf("[MAIN] Operazioni finite\n");
  
  del_gestioneIO (&g);
  return 0;
}

void* produttore(void *arg){
  gestioneIO* g = (gestioneIO*)arg;
  int i;
  buffer buf;
  buf.dato = rand() % 11;
  buf.indirizzo = rand() % 11;
  for(i=0;i<3;i++){
    produci (g, &buf);
    printf("[PROD] Ho prodotto la coppia di valori <%d, %d>\n", buf.dato, buf.indirizzo);
    buf.indirizzo++;
    buf.dato++;
  }
  pthread_exit (NULL);
}

void* consumatore(void *arg){
  gestioneIO *g = (gestioneIO*)arg;
  int i;
  buffer buf;
  for(i=0;i<4;i++){
    sleep(3);
    if(consuma (g, &buf)){
      printf ("[CONS] Il buffer e' vuoto, esco\n");
      break;
    }
    printf("[CONS] Ho prelevato <%d, %d>\n", buf.dato, buf.indirizzo);
  }
  pthread_exit(NULL);
}
