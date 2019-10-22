#include "../monitor/monitor.h"

#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2

#define NOT_FULL 0
#define NOT_EMPTY 1
#define N_VAR 2

#define DIM_BUFFER 100

typedef struct {
  unsigned int fornitore;
  unsigned int stato;
} scaffale;

int *livello_scorte;
int shmid_m, shmid_b ,shmid_ls;
monitor* m;
scaffale* buffer;

void init_buffer();
void del_buffer();

void fornitura(unsigned int*);
void acquisto(unsigned int*);
