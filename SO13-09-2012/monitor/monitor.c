#include "monitor.h"

static void sem_wait(int, int);
static void sem_signal(int, int);

void init_monitor(monitor *m, int n_var){
  int i;
  
  tryerr(m->mutex = semget(IPC_PRIVATE, 1, IPC_CREAT | 0664), "Errore in semget");
  tryerr(semctl(m->mutex, 0, SETVAL, 1), "Errore nel settare la mutex");
  
  tryerr(m->shmid = shmget(IPC_PRIVATE, sizeof(int)*n_var, IPC_CREAT|0664), "Errore in shmget");
  tryerr(m->varcond = (int*)shmat(m->shmid, 0, 0), "Errore in shmat");
  
  tryerr(m->sem_varcond = semget(IPC_PRIVATE, n_var, IPC_CREAT|0664), "Errore in semget");
  for(i=0;i<(m->n_varcond=n_var);i++){
    tryerr(semctl(m->sem_varcond, i, SETVAL, 0), "Errore nel settare i semafori per le wait condition");
    m->varcond[i]=0;
  }
}

void del_monitor(monitor *m){
  tryerr(semctl(m->mutex, 0, IPC_RMID, 0), "Errore nell'eliminare la mutex");
  tryerr(semctl(m->sem_varcond, m->n_varcond, IPC_RMID, 0), "Errore nell'eliminare i semafori per le wait condition");
  tryerr(shmdt(m->varcond), "Errore nel distaccare la memoria condivisa");
  tryerr(shmctl(m->shmid, IPC_RMID, 0), "Errore nell'eliminare le wait condition");
  m->varcond = NULL;
}

void enter_monitor(monitor *m){
  sem_wait(m->mutex, 0);
}

void leave_monitor(monitor *m){
  sem_signal(m->mutex, 0);
}

void wait_condition(monitor *m, int n_var){
  m->varcond[n_var]++;
  sem_signal(m->mutex,0);
  sem_wait(m->sem_varcond, n_var);
  sem_wait(m->mutex, 0);
  m->varcond[n_var]--;
  if(m->varcond[n_var])
    sem_signal (m->sem_varcond, n_var);
}

void signal_condition(monitor *m, int n_var){
  if(m->varcond[n_var])
    sem_signal(m->sem_varcond, n_var);
}

int queue_condition(monitor *m, int n_var){
  return m->varcond[n_var];
}

void sem_wait(int idsem, int nsem){
  struct sembuf sops;
  sops.sem_num = nsem;
  sops.sem_op = -1;
  sops.sem_flg = 0;
  tryerr (semop(idsem, &sops, 1), "Errore in sem_wait");
}

void sem_signal(int idsem, int nsem){
  struct sembuf sops;
  sops.sem_num = nsem;
  sops.sem_op = 1;
  sops.sem_flg = 0;
  tryerr (semop(idsem, &sops, 1), "Errore in sem_signal");
}
