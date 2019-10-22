#ifndef __MONITOR_H
#define  __MONITOR_H

#include "../common.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

typedef struct {
  //semaforo mutex
  int mutex;
  //numero var condition
  int n_varcond;
  //cluster semafori varcondition
  int sem_varcond;
  //id shared memory
  int shmid;
  //array varcondition
  int* varcond;
} monitor;

void init_monitor(monitor*, int);
void del_monitor(monitor*);

void enter_monitor(monitor*);
void leave_monitor(monitor*);

void wait_condition(monitor*, int);
void signal_condition(monitor*, int);
int queue_condition(monitor*, int);

#endif
