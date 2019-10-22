#include <pthread.h>

typedef int elem;

typedef struct {
  elem* dati;
  int dim;
  
  int head;
  
  pthread_mutex_t mutex;
  pthread_cond_t full;
  pthread_cond_t empty;
} stack;

void init_stack(stack*, int);
void del_stack(stack*);

void push_s(stack*, elem);
elem pop_s(stack*);
int get_size(stack*);
