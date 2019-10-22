#ifndef __MSG_H
#define  __MSG_H

#include <sys/types.h>

#define errexit(N, M) printf("%s\n%s\n", M, strerror(errno)), exit(N)
#define tryerr(F, M) if((F)<0) errexit(-1, M)

#define req_len sizeof(m_req)-sizeof(long)
#define res_len sizeof(m_res)-sizeof(long)

#define PATH = "."
#define IDREQ = 22
#define IDRES = 33

typedef struct {
  long type;
  int a;
  int b;
  pid_t pid;
} m_req;

typedef struct {
  long type;
  int res;
} m_res;

#endif
