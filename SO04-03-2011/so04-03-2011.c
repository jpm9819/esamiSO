#include "procedure.h"
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

pid_t spawn(void(*)(void));

int idreq, idres;
int main(){
  int i;
  m_req term;
  pid_t pid;
  
  idreq = msgget (IPC_PRIVATE, IPC_CREAT | 0664);
  idres = msgget (IPC_PRIVATE, IPC_CREAT | 0664);

  term.type = 1l;
  term.a = -1;
  term.b = -1;
  term.pid=getpid ();
  
  printf("Avvio i processi client\n");
  for(i=0;i<N_CLIENT;i++)
    spawn(client);
  printf("Avvio il processo server\n");
  pid = spawn(server);
  printf("Attendo la terminazione dei processi client\n");
  for(i=0;i<N_CLIENT;i++)
    waitpid(-1,NULL,0);
  printf("I processi client sono terminati\nTermino il processo server\n");
  if(msgsnd (idreq, &term, sizeof(m_req)-sizeof(long), 0)<0){
    printf("Errore nella chiusura del server, procedo forzatamente\n%s\n", strerror(errno));
    kill(pid,SIGKILL);
  } else
    waitpid(pid, NULL, 0);
  printf("Fine esecuzione\n");
  msgctl (idreq, IPC_RMID, NULL);
  msgctl (idres, IPC_RMID, NULL);
  return 0;
}

pid_t spawn(void(*f)(void)){
  pid_t pid;
  if((pid=fork())<0){
    printf ("Errore nella creazione dei proessi\n%s\n", strerror(errno));
    exit(1);
  }
  if(pid==0) f();
  return pid;
}
