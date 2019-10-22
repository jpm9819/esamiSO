#ifndef MON_H
#define MON_H

#include <pthread.h>

#define MAX_OP 4
#define MAX_ELAB  5

#define LIBERO 0
#define IN_USO 1
#define OCCUPATO 2

typedef struct{
  int operandi[MAX_OP];
  int totale_operandi;
}buffer;

typedef struct{
  buffer elaborazioni[MAX_ELAB];
  int stato[MAX_ELAB];
  
  int n_occupati;
  int n_liberi;
  
  pthread_mutex_t mutex;
  pthread_cond_t produttori;
  pthread_cond_t consumatori;
}Monitor_elaborazioni;

void init_monitor(Monitor_elaborazioni*);
void del_monitor(Monitor_elaborazioni*);

void produci_richiesta(Monitor_elaborazioni*, buffer*);
int consuma_richiesta(Monitor_elaborazioni*, buffer*);

#endif
