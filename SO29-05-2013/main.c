#include "msg.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define N_CLIENT 5

pid_t spawn(char*);

int main(){
  pid_t pid_s;
  int i, reqid, resid;
  key_t key_req, key_res;
  m_req headshot;
  
  printf("[MAIN] Inizializzo le code\n");

  key_req = ftok (".", 'Q');
  key_res = ftok (".", 'S');
  
  tryerr(reqid = msgget(key_req, IPC_CREAT | 0664), "Errore in apertura delle code nel main");
  tryerr(resid = msgget(key_res, IPC_CREAT | 0664), "Errore in apertura delle code nel main");
  
  printf("[MAIN] Inizializzo il server\n");
  pid_s = spawn ("./server");
  printf("[MAIN] Inizializzo i client\n");
  for(i=0;i<N_CLIENT;i++)
    spawn("./client");
  
  printf("[MAIN] Attendo la terminazione dei client\n");

  headshot.a=-1;
  headshot.b=-1;
  headshot.type = 1l;
  headshot.pid=0;

  for(i=0;i<N_CLIENT;i++)
    waitpid (-1, NULL, 0);
  
  printf("[MAIN] Sto uccidendo il server\n");
  
  sleep(3);
  
  if(msgsnd(reqid, &headshot, req_len, 0)<0){
    printf ("[MAIN] Errore nell'uccisione del server\nProcedo alla vecchia maniera\n");
    kill (pid_s, SIGKILL);
  }
  waitpid (pid_s, NULL, 0);
            
  tryerr (msgctl(reqid, IPC_RMID, 0), "Errore nell'eliminazione delle code del main");
  tryerr (msgctl(resid, IPC_RMID, 0), "Errore nell'eliminazione delle code del main");
            
  return 0;
}

pid_t spawn(char* nfile){
  pid_t out;
  tryerr(out = fork(), "Errore nella fork");
  if(out == 0) execlp (nfile, nfile, 0);
  return out;
}
