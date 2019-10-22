#include "monitor.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define N_RIC 4
#define N_ELA 2

void* richiedente(void*);
void* elaboratore(void*);

int main(){
  pthread_t threads[N_RIC+N_ELA];
  int i;
  Monitor_elaborazioni m;
  
  init_monitor (&m);
  
  srand (time (NULL));
  
  for(i=0;i<N_RIC;i++){
    pthread_create (&threads[i], NULL, richiedente, (void*)&m);
    printf("[main] creato produttore %d\n", i); 
  }
  for(;i<N_RIC+N_ELA;i++){
    pthread_create (&threads[i], NULL, elaboratore, (void*)&m);
    printf ("[main] creato elaboratore %d\n", i-N_RIC);
  }
  for(i=0;i<N_RIC+N_ELA;i++)
    pthread_join (threads[i], NULL);
  
  del_monitor (&m);
}

void* richiedente(void* ptr){
  Monitor_elaborazioni *m = (Monitor_elaborazioni*)ptr;
  int i, k, n_op;
  buffer b;
  
  for(i=0;i<3;i++){
    n_op= rand() % 3 + 2;
    if(n_op<2) n_op=2;
    else if (n_op>4) n_op = 4;
    b.totale_operandi=n_op;
    for(k=0;k<n_op;k++)
      b.operandi[k] = rand() % 11;
    produci_richiesta (m, &b);
    printf("[PRODUTTORE] RICHIESTA PRODOTTA\n");
  }
  
  pthread_exit (NULL);
}

void* elaboratore(void* ptr){
  Monitor_elaborazioni *m = (Monitor_elaborazioni*)ptr;
  int i, k, somma;
  buffer b;
  char buff[40], tmp[7];
  
  for(i=0;i<6;i++){
    somma = consuma_richiesta (m, &b);
    sleep(b.totale_operandi);
    sprintf(buff, "[ELABORATORE] %d ", b.operandi[0]);
    for(k=1;k<b.totale_operandi;k++){
      sprintf(tmp, "+ %d ", b.operandi[k]);
      strcat (buff, tmp);
    }
    printf("%s= %d\n", buff, somma);
  }
  
  pthread_exit (NULL);
}
