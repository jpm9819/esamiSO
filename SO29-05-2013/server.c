#include "msg.h"
#include "shared_buffer.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define N_WORKER 2

void* thread_manager (void*);
void* thread_worker (void*);

pthread_t worker[N_WORKER];
int reqid, resid;
pthread_mutex_t res_mutex;

int main(){
  key_t key_req, key_res;
  shared_buffer s;
  pthread_t manager;
  pthread_attr_t worker_attr;
  int i;
  
  key_req = ftok (".", 'Q');
  key_res = ftok (".", 'S');
  
  init_shared_buffer (&s);
  pthread_mutex_init(&res_mutex, NULL);
  
  tryerr(reqid = msgget (key_req, IPC_CREAT | 0664), "Errore nella creazione delle code");
  tryerr(resid = msgget (key_res, IPC_CREAT | 0664), "Errore nella creazione delle code");
  
  printf ("[SERVER] Sto inizializzando il manager\n");
  pthread_create (&manager, NULL, thread_manager, (void*)&s);
  
  printf("[SERVER] Sto inizializzando %d worker\n", N_WORKER);
  pthread_attr_init (&worker_attr);
  pthread_attr_setdetachstate (&worker_attr, PTHREAD_CREATE_DETACHED);
  for(i=0;i<N_WORKER;i++)
    pthread_create (&worker[i], &worker_attr, thread_worker, (void*)&s);
//    pthread_create (&worker[i], NULL, thread_worker, (void*)&s);
  pthread_attr_destroy (&worker_attr);
  
  printf("[SERVER] Sto aspettando la fine del manager\n");
  
  pthread_join(manager, NULL);
  printf("[SERVER] Il manager e' terminato correttamente \n");
  
  pthread_mutex_destroy (&res_mutex);
  del_shared_buffer (&s);
  return 0;
}

void* thread_manager(void *param){
  shared_buffer *s = (shared_buffer*)param;
  m_req req;
  int i;
  
  while(1){
    if(msgrcv(reqid, &req, req_len, 0, IPC_NOWAIT)<0){
      if(errno == ENOMSG) printf ("[MANAGER] Non ci sono messaggi in coda, attendo 3 secondi\n") ,sleep(3);
      else errexit(-1, "Errore di ricezione in thread_manager");
    } else {
      if((req.a==-1)&&(req.b==-1)){
        for(i=0;i<N_WORKER;i++){
          printf("Sto uccidendo il worker %d\n", i);
          pthread_cancel (worker[i]);
//          pthread_join(worker[i], NULL); 
        }
        pthread_exit(NULL);
      }
      printf("[MANAGER] Trovato messaggio in coda, metto in buffer\n");
      enqueue_sb (s, &req);
    }
  }
}

void* thread_worker(void *param){
  shared_buffer *s = (shared_buffer*)param;
  m_req req;
  m_res res;
  pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  
  while(1){
    pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
    dequeue_sb (s, &req);
    pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, NULL);
    printf("[WORKER] Trovato messaggio in coda\n");
    res.type = (long)req.pid;
    res.res = req.a * req.b;
    printf("[WORKER] Sto mandando risposta al client %ld\n", res.type);
    pthread_mutex_lock (&res_mutex);
    tryerr (msgsnd (resid, &res, res_len, 0), "Errore di invio nel worker\n");
    pthread_mutex_unlock (&res_mutex);
  }
}
