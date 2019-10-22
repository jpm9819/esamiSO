#ifndef __MONITOR_H
#define  __MONITOR_H

typedef struct {
  //id sem monitor
  int sem;
  //id sem urgente monitor
  int sem_urg;
  //dim array varcond
  int dim_varcond;
  //id shared memory array varcond
  int shm_id;
  //id sem varcond
  int varcond_sem;
  //array varcond
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
