#include <pthread.h>

#define DIM_IO 10

typedef struct {
  int indirizzo;
  int dato;
} buffer;

typedef struct {
  buffer buf[DIM_IO];
  
  unsigned short dim;
  unsigned short head;
  unsigned short tail;
  
  pthread_mutex_t mutex;
  pthread_cond_t not_full;
} gestioneIO;

void init_gestioneIO(gestioneIO*);
void del_gestioneIO(gestioneIO*);

void produci(gestioneIO*, buffer*);
int consuma(gestioneIO*, buffer*);
