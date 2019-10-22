#include "scaffale/scaffale.h"
#include "utility.h"
#include <sys/wait.h>
#include <unistd.h>

#define N_FOR 10
#define N_CLI 10

void fornitore();
void cliente();
void spawn(void(*)());

int i;

int main(){
  init_buffer ();
  printf("[MAIN] Sto inizializzando i fornitori\n");
  for(i=0;i<N_FOR;i++)
    spawn(fornitore);
  printf("[MAIN] Sto inizializzando i clienti\n");
  for(i=0;i<N_CLI;i++)
    spawn(cliente);
  printf("[MAIN] Sto attendendo la fine delle operazioni\n");
  for(i=0;i<N_FOR+N_CLI;i++)
    waitpid(-1, NULL, 0);
  printf ("[MAIN] Operazioni finite\n");
  del_buffer ();
  return 0;
}

void spawn(void(*f)()){
  pid_t pid;
  tryerr(pid = fork(), "Errore nella fork");
  if(pid!=0) return;
  f();
  exit(0);
}

void fornitore(){
  unsigned int pid = (unsigned int) getpid();
  for(i=0;i<15;i++){
    fornitura(&pid);
    printf("[FORNITORE %d] Ho fornito\n", pid);
    sleep(1);
  }
}

void cliente(){
  unsigned int pid;
  for(i=0;i<15;i++){
    acquisto (&pid);
    printf("[CLIENTE %d] Ho acquistato %d\n", pid, getpid());
    sleep(1);
  }
}
