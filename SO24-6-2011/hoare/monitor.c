#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "monitor.h"

static void sem_wait(int id_sem, int n_sem);
static void sem_signal(int id_sem, int n_sem);
static int sem_queue(int id_sem, int n_sem);

void init_monitor(monitor *m, int dim){
  int i;
  
  m->sem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0664);
  semctl (m->sem, 0, SETVAL, 1);
  
  m->sem_urg = semget(IPC_PRIVATE, 1, IPC_CREAT | 0664);
  semctl (m->sem_urg, 0, SETVAL, 0);
  
  m->varcond_sem = semget (IPC_PRIVATE, dim, IPC_PRIVATE | 0664);
  for(i=0;i<dim;i++)
    semctl (m->varcond_sem, i, SETVAL, 0);
  
  m->dim_varcond = dim;
  
  m->shm_id = shmget (IPC_PRIVATE, sizeof(int)*dim, IPC_CREAT | 0664);
  m->varcond = (int*)shmat (m->shm_id, 0, 0);
  
  for(i=0;i<dim;i++)
    m->varcond[i] = 0;
}

void del_monitor(monitor *m){
  semctl (m->sem, 0, IPC_RMID, 0);
  semctl (m->sem_urg, 0, IPC_RMID, 0);
  semctl (m->varcond_sem, m->dim_varcond, IPC_RMID, 0);
  shmctl (m->shm_id, IPC_RMID, 0);
}

void enter_monitor(monitor *m){
  sem_wait (m->sem, 0);
}

void leave_monitor(monitor *m){
  sem_signal (m->sem, 0);
}

void wait_condition(monitor *m, int id_cond){
  m->varcond[id_cond]++;
  leave_monitor (m);
  sem_wait (m->varcond_sem, id_cond);
  enter_monitor (m);
  m->varcond[id_cond]--;
}

void signal_condition(monitor *m, int id_cond){
  if(m->varcond[id_cond]>0){
    sem_signal (m->varcond_sem, id_cond);
  }
}

int queue_condition(monitor* m, int id_cond){
  return m->varcond[id_cond];
}

void sem_wait(int id_sem, int n_sem){
  struct sembuf sem_buf;
  sem_buf.sem_flg = 0;
  sem_buf.sem_op = -1;
  sem_buf.sem_num = n_sem;
  semop (id_sem, &sem_buf, 1);
}

void sem_signal(int id_sem, int n_sem){
  struct sembuf sem_buf;
  sem_buf.sem_flg = 0;
  sem_buf.sem_op = 1;
  sem_buf.sem_num = n_sem;
  semop (id_sem, &sem_buf, 1);
}

int sem_queue(int id_sem, int n_sem){
  return semctl (id_sem, n_sem, GETVAL);
}
