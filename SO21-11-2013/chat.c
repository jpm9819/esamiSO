#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define errexit(N, M) printf("%s\n%s\n", M, strerror(errno)), exit(N)
#define tryerr(F, M) if((F)<0) errexit(-1, M)

typedef struct {
  long type;
  char buf[256];
} msg;
                       
void* in(void*);
void* out(void*);

int outid, inid;
pthread_t thread[2];
pthread_t maint;
int main(int argc, char** argv){
  key_t out_k, in_k;
  pthread_attr_t attr;
  
  if(argc<3){
    printf("Uso: <%s> <send_key> <recv_key>", argv[0]);
    exit(1);
  }

  out_k = (key_t)atoi(argv[1]);
  in_k = (key_t)atoi(argv[2]);
  
  tryerr(outid = msgget(out_k, IPC_CREAT | 0664), "Errore creazione coda");
  tryerr(inid = msgget(in_k, IPC_CREAT | 0664), "Errore creazione coda");
  
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  maint = pthread_self ();
  pthread_create (&thread[0], &attr, in, NULL);
  pthread_create (&thread[1], &attr, out, NULL);
  
  pthread_exit(NULL);
  
  return 0;
}

void* in(void *null){
  msg m;
  
  pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  while(1){
    msgrcv (inid, &m, sizeof(msg)-sizeof(long), 0, 0);
    if(!strcmp(m.buf,"exit")){
      pthread_cancel(thread[1]);
      pthread_exit(NULL);
    }
    printf("%s\n", m.buf);
  }
}

void* out(void *null){
  msg m;
  m.type = 1l;
  
  pthread_join(maint, NULL);
  pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  while(1){
    scanf("%256c", m.buf);
    msgsnd(outid, &m, sizeof(msg)-sizeof(long), 0);
    if(!strcmp(m.buf, "exit")){
      pthread_cancel(thread[0]);
      pthread_exit(NULL);
    }
  }
}
