#include <pthread.h>

#define DIM 3
#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2

typedef struct {
  int id;
  int quota;
} volo;

typedef struct {
  volo vet_voli[DIM];
  int stato[DIM];
  
  int n_liberi;
  
  pthread_cond_t produttori;
  pthread_mutex_t mutex;
} gestioneVoli;

void init_gestioneVoli(gestioneVoli*);
void del_gestioneVoli(gestioneVoli*);

void inserisciVolo(gestioneVoli*,int);
void rimuoviVolo(gestioneVoli*,int);
void aggiornaVolo(gestioneVoli*,int,int);
