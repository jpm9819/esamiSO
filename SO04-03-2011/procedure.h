#include <sys/types.h>

#define N_MSG 5
#define N_CLIENT 3

typedef struct {
  long type;
  pid_t pid;
  int a;
  int b;
} m_req;

typedef struct {
  long type;
  int res;
} m_res;

void client();
void server();
void* prodotto(void*);
