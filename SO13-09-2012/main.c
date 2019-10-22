#include "priorityBuffer/priorityBuffer.h"
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define N_PRODH 1
#define N_PRODL 3
#define N_CONS 1
#define N_PROC 5

void prodHP();
void prodLP();
void consumatore();
void spawn(void(*)());

priorityBuffer *s;
int i;
int main(){
  int shmid;
  srand(time(NULL));
  tryerr(shmid = shmget(IPC_PRIVATE, sizeof(priorityBuffer), IPC_CREAT|0664), "Errore nell'instanziare l'array a priorita'");
  tryerr(s = (priorityBuffer*)shmat(shmid, 0, 0), "Errore nell' attaccare la memoria condivisa");
  
  init_priorityBuffer(s);
  
  printf("[MAIN] Sto creando i produttori hp\n");
  for(i=0;i<N_PRODH;i++)
    spawn (prodHP);
  printf("[MAIN] Sto creando i produttori lp\n");
  for(i=0;i<N_PRODL;i++)
    spawn(prodLP);
  printf("[MAIN] Sto creando i consumatori\n");
  for(i=0;i<N_CONS;i++)
    spawn(consumatore);
  printf("[MAIN] Sto attendendo il termine dei processi\n");
  for(i=0;i<N_PROC;i++)
    waitpid(-1, NULL, 0);
  printf("[MAIN] Fine delle operazioni\n");
  
  del_priorityBuffer(s);
  tryerr(shmctl(shmid, IPC_RMID, 0), "Errore nella rimozione della shared memory");
  return 0;
}

void spawn(void (*f)()){
  pid_t pid;
  tryerr (pid = fork(), "Errore nella fork");
  if(pid!=0) return;
  f();
  exit(0);
}

void prodHP(){
  for(i=0;i<3;i++){
    printf("[PROD_H] Sto producendo\n");
    prod_HP(s);
    sleep(2);
  }
}

void prodLP(){
  for(i=0;i<3;i++){
    printf("[PROD_L] Sto producendo\n");
    prod_LP(s);
    sleep(1);
  }
}

void consumatore(){
  for(i=0;i<12;i++){
    printf("[CONS] Sto consumando\n");
    consuma(s);
    sleep(1);
  }
}
