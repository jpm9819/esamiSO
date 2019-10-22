#ifndef __MONITOR_H
#define  __MONITOR_H

typedef struct {
  int mutex;
  int shmid;
  int n_cond;
  int sem_cond;
  int* cond_count;
} monitor;

void init_monitor(monitor*, int);
void del_monitor(monitor*);

void enter_monitor(monitor*);
void leave_monitor(monitor*);

void wait_condition(monitor*,int);
void signal_condition(monitor*,int);

#endif
