#include "data_structure.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void* generatore(void* ptr){
  mon_vet *m = (mon_vet*)ptr;
  elem x;
  int i;
  
  for(i=0;i<10;i++){
    x.a=rand() % 11;
    x.b=rand() % 11;
    
    genera (m, x);
  }
  pthread_exit (NULL);
}

void* aggiornatore(void* ptr){
  mon_vet *vet = ((mon*)ptr)->vet;
  mon_buf *buf = ((mon*)ptr)->buf;
  int i;
  
  for(i=0;i<10;i++,sleep(1))
    aggiorna (buf, preleva (vet));
  
  pthread_exit(NULL);
}

void* consultatore(void* ptr){
  mon_buf *m = (mon_buf*)ptr;
  int i;
  for(i=0;i<6;i++,sleep (2))
    consulta (m);
  pthread_exit (NULL);
}
