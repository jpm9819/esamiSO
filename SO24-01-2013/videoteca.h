#ifndef __VIDEOTECA_H
#define  __VIDEOTECA_H

#include <pthread.h>

#define N_DVD 6
#define N_COPIE 2
#define N_ID 3

#define DISPONIBILE 0
#define AFFITTATO 1

typedef struct {
  int id_film;
  int id_copia;
  int stato;
} DVD;

typedef struct {
  DVD dvd[6];
  
  pthread_cond_t ok_affitta[N_ID];
  
  int n_disp;
  int n_affit;
  
  pthread_mutex_t mutex;
} monitor;

void init_monitor(monitor*);
void del_monitor(monitor*);

int affitta(monitor*, int);
void restituisci(monitor*,int,int);
void stampa(monitor*);

#endif

