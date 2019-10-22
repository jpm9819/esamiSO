#include "../utility.h"
#include "monitor.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

static void wait_sem(int, unsigned short);
static void signal_sem(int, unsigned short);

void init_monitor(monitor *m, int n_var){
  int i;
  
  tryerr(m->mutex = semget(IPC_PRIVATE, 1, IPC_CREAT|0664), "Errore creazione mutex");
  tryerr(semctl(m->mutex, 0, SETVAL, 1), "Errore setting mutex");
  
  m->n_cond=n_var;
  
  tryerr(m->sem_cond = semget(IPC_PRIVATE, n_var, IPC_CREAT|0664), "Errore nella creazione sem_cond");
  
  tryerr(m->shmid = shmget(IPC_PRIVATE, sizeof(int)*n_var, IPC_CREAT|0664), "Errore nella creazione shm varcond");
  tryerr(m->cond_count=(int*)shmat(m->shmid, 0, 0), "Errore nell' attaccamento della memoria condivisa varcond");
  
  for(i=0;i<n_var;i++){
    tryerr(semctl(m->sem_cond, i, SETVAL, 0), "Errore setting sem_varcond");
    m->cond_count[i]=0;
  }
}

void del_monitor(monitor *m){
  tryerr(semctl(m->mutex, 0, IPC_RMID, 0), "Errore elimiazione mutex");
  tryerr(semctl(m->sem_cond, m->n_cond, IPC_RMID, 0), "Errore eliminazione sem_cond");
  tryerr(shmdt(m->cond_count), "Errore distaccamento cond_count");
  tryerr(shmctl(m->shmid, IPC_RMID, 0), "Errore eliminazione shm varcond");
  m->cond_count = NULL;
}

void enter_monitor(monitor *m){
  wait_sem(m->mutex, 0);
}

void leave_monitor(monitor *m){
  signal_sem(m->mutex, 0);
}

void wait_condition(monitor *m, int var){
  m->cond_count[var]++;
  signal_sem(m->mutex, 0);
  wait_sem(m->sem_cond, var);
  wait_sem(m->mutex, 0);
  if(--m->cond_count[var])
    signal_sem(m->sem_cond, var);
}

void signal_condition(monitor *m, int var){
  if(m->cond_count[var])
    signal_sem(m->sem_cond, var);
}

void wait_sem(int semid, unsigned short nsem){
  struct sembuf sop;
  sop.sem_op = -1;
  sop.sem_flg = 0;
  sop.sem_num = nsem;
  tryerr(semop(semid, &sop, 1), "Errore in wait_sem");
}

void signal_sem(int semid, unsigned short nsem){
  struct sembuf sop;
  sop.sem_op = 1;
  sop.sem_flg = 0;
  sop.sem_num = nsem;
  tryerr(semop(semid, &sop, 1), "Errore in signal_sem");  
}
