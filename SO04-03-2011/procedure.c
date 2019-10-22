#include "procedure.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

extern int idreq, idres;
void client(){
  int i;
  m_req req;
  m_res res;
  
  req.type=1;
  req.pid=getpid ();
  
  srand(getpid());
  for(i=0;i<N_MSG;i++){
    req.a = rand() % 101;
    req.b = rand() % 101;
    if(msgsnd(idreq, &req, sizeof(m_req)-sizeof(long), 0)<0){
      printf("Errore del client %d nell'invio\n%s\n", getpid(), strerror(errno));
      exit(1);
    }
    printf("Il client %d ha inviato la coppia di interi %d,%d\n", getpid(),req.a, req.b);
    if(msgrcv(idres, &res, sizeof(m_res)-sizeof(long), (long)getpid(), 0)<0){
      printf("Errore del client %d nella ricezione\n%s\n", getpid(), strerror(errno));
      exit(1);
    }
    printf("Il client %d ha ricevuto %d come risposta\n", getpid(), res.res);
  }
  exit(0);
}

pthread_mutex_t mutex;
void server(){
  pthread_t proc;
  pthread_attr_t attr;
  m_req* req;
  
  pthread_mutex_init(&mutex, NULL);
  pthread_attr_init (&attr);
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
  
  while(1){
    req=(m_req*)malloc(sizeof(m_req));
    if(msgrcv (idreq, req, sizeof(m_req)-sizeof(long), 0, 0)<0){
      printf("Il server ha fallito nella ricezione della richiesta \n%s\n", strerror(errno));
      free(req);
      exit(1);
    }
    if((req->a==-1) && (req->b==-1)){
      printf("Il server sta terminando la sua esecuzione\n");
      free(req);
      exit(0);
    }
    printf("Il server sta creando un thread per rispondere\n");
    pthread_create(&proc, &attr, prodotto, (void*)req);
  }
}

void* prodotto(void* ptr){
  m_req* req = (m_req*)ptr;
  m_res res;
  
  res.res = req->a * req->b;
  res.type = (long)req->pid;
  pthread_mutex_lock(&mutex);
  printf("Il thread sta rispondendo alla richiesta di %d\n", req->pid);
  if(msgsnd(idres, &res, sizeof(m_res)-sizeof(long),0)<0){
    printf("Il thread ha fallito nel rispondere\n%s\n", strerror(errno));
  }
  pthread_mutex_unlock(&mutex);
  free(req);
  pthread_exit(NULL);
}
