#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_PROD 5
#define N_CONS 1

void* producer(void*);
void* consumer(void*);

int main(){
  pthread_t threads[N_CONS+N_PROD];
  stack s;
  int i;
  
  init_stack (&s, 6);
  
  for(i=0;i<N_PROD;i++)
    pthread_create (&threads[i], NULL, producer, (void*)&s);
  for(;i<N_PROD+N_CONS;i++)
    pthread_create (&threads[i], NULL, consumer, (void*)&s);
  for(i=0;i<N_PROD+N_CONS;i++)
    pthread_join(threads[i], NULL);
  
  del_stack (&s);
  
  return 0;
}

void* producer(void* ptr){
  stack *s = (stack*)ptr;
  int i, value;
  
  for(i=0;i<4;i++){
    value = (rand() % 11);
    push_s (s, value);
    printf("[PRODUCER] ho inserito %d\n", value);
    sleep(1);
  }
                           
  pthread_exit(NULL);
}

void* consumer(void* ptr){
  stack *s = (stack*)ptr;
  int i, a, b;
  
  for(i=0;i<10;i++){
    a = pop_s (s);
    b = pop_s (s);
    printf("[CONSUMER] %d + %d = %d\n", a, b, a+b);
  }
  
  pthread_exit(NULL);
}
