#ifndef __UTILITY_H
#define  __UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define errexit(N, M) printf("%m\n%s\n", strerror(errno)), exit(N)
#define tryerr(F, M) if((F)<0) errexit(-1, M)

#endif
