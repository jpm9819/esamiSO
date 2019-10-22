#include "shared/shared_mem.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define N_CLIENT 5
#define DIM_DISCO 20

void Schedulatore();
void Client();

shared_queue *s;

int main(){
  int sq_id, i;
  
  srand(time(NULL));
  
  sq_id = shmget (IPC_PRIVATE, sizeof(shared_queue), IPC_CREAT | 0664);
  s = (shared_queue*)shmat(sq_id,0,0);
  init_shared_queue (s);
  
  printf("[MAIN] Inizializzo lo schedulatore\n");
  if(fork()==0){
    Schedulatore ();
    exit(0);
  }
  
  printf("[MAIN] Inizializzo i client\n");
  for(i=0;i<N_CLIENT;i++)
    if(fork()==0){
      printf("\t[MAIN] Client %d inizializzato\n", i);
      Client ();
      exit(0);
    }
  
  printf("[MAIN] Attendo la terminazione dei processi figli\n");
  
  for(i=0;N_CLIENT+1;i++)
    waitpid(-1, NULL, 0);
  
  del_shared_queue (s);
  shmctl(sq_id, IPC_RMID, 0);
  return 0;
}

void Client(){
  int i;
  richiesta ric;
  ric.pid = getpid();
  
  for(i=0;i<5;i++){
    ric.posizione = rand() % DIM_DISCO;
    printf("[CLIENT %d] Sto richiedendo la posizione %u\n", ric.pid, ric.posizione);
    enqueue_sq (s, &ric);
  }
               
  exit(0);
}

void printArray(pid_t* buf){
  int i;
  char buffer[(7*DIM_DISCO)+1] = {'\0'};
  char placeholder[8];
  for(i=0; i<DIM_DISCO; i++){
    sprintf (placeholder, "|%5u|", buf[i]);
    strcat(buffer, placeholder);
  }
  printf("%s\n", buffer);
}

void Schedulatore(){
  unsigned int prevPos = 0, i;
  int t;
  richiesta ric;
  pid_t disco[DIM_DISCO];
  memset (disco, 0, sizeof(pid_t)*DIM_DISCO);
  printArray (disco);
  for(i=0;i<25;i++){
    dequeue_sq (s, &ric);
    printf("[SCHED]Ho ricevuto la richiesta, elaboro\n");
    t = prevPos - ric.posizione;
    t = (t<0) ? -t : t;
    printf("[SCHED] Devo operare per %d secondi\n", t);
    sleep(t);
    prevPos = ric.posizione;
    disco[ric.posizione] = ric.pid;
    printf("[SHED]Elaborazione completata, disco attuale:\n");
    printArray (disco);
  }
  
  exit(0);
}
