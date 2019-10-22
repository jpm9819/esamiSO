#include "../utility.h"
#include "scaffale.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

static int find_free();
static int find_occ();

void init_buffer(){
  int i;
  
  tryerr(shmid_ls = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0664), "Errore allocazione livello_scorte");
  tryerr(livello_scorte = (int*)shmat(shmid_ls, 0, 0), "Errore assegnazione memoria livello_scorte");
  
  *livello_scorte = 0;
  
  tryerr(shmid_m = shmget(IPC_PRIVATE, sizeof(monitor), IPC_CREAT|0664), "Errore allocazione monitor");
  tryerr(m = (monitor*)shmat(shmid_m, 0, 0), "Errore assegnazione memoria monitor");
  
  tryerr(shmid_b = shmget(IPC_PRIVATE, sizeof(buffer)*DIM_BUFFER, IPC_CREAT|0664), "Errore allocazione buffer");
  tryerr(buffer = (scaffale*)shmat(shmid_b, 0, 0), "Errore assegnazione memoria buffer");
  
  init_monitor(m, N_VAR);
  
  for(i=0;i<DIM_BUFFER;i++){
    buffer[i].fornitore = 0;
    buffer[i].stato = LIBERO;
  }
}

void del_buffer(){
  del_monitor(m);
  
  tryerr(shmdt(m), "Errore distaccamento memoria monitor");
  tryerr(shmdt(buffer), "Errore distaccamento memoria buffer");
  tryerr(shmdt(livello_scorte), "Errore distazzamento memoria livello_scorte");
  
  tryerr(shmctl(shmid_m, IPC_RMID, 0), "Errore eliminazione memoria monitor");
  tryerr(shmctl(shmid_b, IPC_RMID, 0), "Errore eliminazione memoria buffer");
  tryerr(shmctl(shmid_ls, IPC_RMID, 0), "Errore eliminazione memoria livello_scorte");
  
  m = NULL;
  buffer = NULL;
  livello_scorte = NULL;
}

void fornitura(unsigned int *id){
  int id_scaffale;
  enter_monitor(m);
  while(*livello_scorte==DIM_BUFFER)
    wait_condition (m, NOT_FULL);
  id_scaffale = find_free();
  buffer[id_scaffale].stato = IN_USO;
  leave_monitor(m);
  buffer[id_scaffale].fornitore = *id;
  sleep(2);
  enter_monitor(m);
  *livello_scorte = *livello_scorte +1;
  buffer[id_scaffale].stato = OCCUPATO;
  signal_condition (m, NOT_EMPTY);
  leave_monitor(m);
}

void acquisto(unsigned int *id){
  int id_scaffale;
  enter_monitor (m);
  while(*livello_scorte==0)
    wait_condition (m, NOT_EMPTY);
  id_scaffale = find_occ();
  buffer[id_scaffale].stato = IN_USO;
  *livello_scorte = *livello_scorte -1;
  leave_monitor (m);
  *id = buffer[id_scaffale].fornitore;
  buffer[id_scaffale].fornitore = 0;
  sleep(2);
  enter_monitor(m);
  buffer[id_scaffale].stato = LIBERO;
  signal_condition (m, NOT_FULL);
  leave_monitor (m);
}

int find_free(){
  int i;
  for(i=0;i<DIM_BUFFER;i++)
    if(buffer[i].stato==LIBERO)
      return i;
  return -1;
}

int find_occ(){
  int i;
  for(i=0;i<DIM_BUFFER;i++)
    if(buffer[i].stato == OCCUPATO)
      return i;
  return -1;
}
