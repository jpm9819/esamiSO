#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define errexit(N, M) printf("%s\n%s\n", M, strerror(errno)), exit(N)
#define tryerr(F, M) if((F)<0) errexit(-1, M)

#define N_CLIENT 5

typedef struct {
  long type;
  pid_t pid;
} msg;

typedef struct{
  long type;
  pid_t buffer[10];
} msg_p;

void server();
void client();
void printer();
int spawn(void(*)());

int msqid_c;
int msqid_p;
int i;
int main(){
  pid_t pidserver;
  pid_t pidprinter;
  
  tryerr (msqid_c = msgget(IPC_PRIVATE, IPC_CREAT | 0664), "Errore nella creazione delle msgqueue");
  tryerr(msqid_p = msgget(IPC_PRIVATE, IPC_PRIVATE | 0664), "Errore nella creazione della queue printer");
  
  printf("[MAIN] Inizializzo i processi\n");
  for(i=0;i<N_CLIENT;i++)
    spawn(client);
  pidserver = spawn(server);
  pidprinter = spawn(printer);
  printf("[MAIN] Attendo la fine delle operazioni\n");
  for(i=0;i<N_CLIENT;i++)
    waitpid (-1, NULL, 0);
  kill(pidserver, SIGKILL);
  kill (pidprinter, SIGKILL);
  printf ("[MAIN] Operazioni finite\n");
  
  tryerr (msgctl(msqid_c, IPC_RMID, 0), "Errore eliminazione code");
  tryerr(msgctl(msqid_p, IPC_RMID, 0), "Errore eliminazione code");
  return 0;
}

void client(){
  msg m;
  m.type = 1l;
  m.pid = getpid ();
  for(i=0;i<15;i++){
    tryerr(msgsnd(msqid_c, &m, sizeof(msg)-sizeof(long), 0), "Errore di invio nel client");
    sleep(1);
  }
}

void server(){
  msg m;
  msg_p p;
  p.type = 1l;
  i=0;
  while(1){
    tryerr(msgrcv(msqid_c, &m, sizeof(msg)-sizeof(long),0 ,0), "Errore di ricezione nel server");
    p.buffer[i] = m.pid;
    i = (i+1) % 10;
    if(!i)
      tryerr(msgsnd(msqid_p, &p, sizeof(msg_p)-sizeof(long), 0), "Errore di invio nel server");
  }
}

void printer(){
  msg_p m;
  while(1){
    tryerr(msgrcv(msqid_p, &m, sizeof(msg_p)-sizeof(long), 0, 0), "Errore ricezione in printer");
    for(i=0;i<10;i++)
      printf("%d ",m.buffer[i]);
    printf("\n");
  }
}

pid_t spawn(void(*f)()){
  pid_t out;
  tryerr(out = fork(), "Errore nella fork");
  if(out)return out;
  f();
  exit(1);
}
