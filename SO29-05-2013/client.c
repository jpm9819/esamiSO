#include "msg.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define N_REQ 5
//#define DEBUG_H

int main(){
  key_t key_req, key_res;
  int i, reqid, resid;
  m_res res;
  m_req req;
  
  key_req = ftok (".", 'Q');
  key_res = ftok (".", 'S');
  
  tryerr(reqid = msgget (key_req, IPC_CREAT | 0664), "Errore di apertura della coda nel client");
  tryerr(resid = msgget (key_res, IPC_CREAT | 0664), "Errore di apertura della coda nel client");
  
  srand(getpid ());
  req.type = 1l;
  req.pid = getpid ();
  
  for(i=0;i<N_REQ;i++){
    req.a = rand() % 11;
    req.b = rand() & 11;
    printf("[CLIENT %d] Sto mandando la richiesta %d al server <%d * %d>\n", getpid (), i, req.a, req.b);
    tryerr (msgsnd (reqid, &req, req_len, 0), "Errore di invio del client");
#ifdef DEBUG_H
    printf("[CLIENT][DEBUG] Sto ricevendo %ld dalla coda\n", (long)getpid());
#endif
    tryerr (msgrcv (resid, &res, res_len, (long)getpid (), 0), "Errore di ricezione del client");
    printf("[CLIENT %d] Ho ricevuto la risposta %d\n", getpid(), res.res);
  }
  
  printf("[CLIENT %d] Ho terminato\n", getpid());
  return 0;
}
