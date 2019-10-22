#include <pthread.h>

#define DIM 5

typedef struct {int a; int b; } elem;

typedef struct{
  elem vet[5];
  int head, tail;
  int count;
  pthread_mutex_t mutex;
  pthread_cond_t not_full;
  pthread_cond_t not_empty;
} mon_vet;

typedef struct{
  elem buf;
  int occupato;
  int nlettori;
  pthread_mutex_t mutex;
  pthread_cond_t libero;
}mon_buf; 

typedef struct {
  mon_buf *buf;
  mon_vet *vet;
} mon;

void init_mon_buf(mon_buf*);
void init_mon_vet(mon_vet*);
void rem_mon_buf(mon_buf*);
void rem_mon_vet(mon_vet*);

void genera(mon_vet*,elem);
elem preleva(mon_vet*);
void aggiorna(mon_buf*, elem);
void consulta(mon_buf*);

void* generatore(void* ptr);
void* aggiornatore(void* ptr);
void* consultatore(void* ptr);
