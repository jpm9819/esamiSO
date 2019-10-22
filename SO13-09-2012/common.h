#ifndef __COMMON_H
#define  __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*--------------------------- UTILITY MACRO ----------------------*/

#define errexit(N, M) printf("%s\n%s\n", M, strerror(errno)) , exit(N)
#define tryerr(F, M) if((F)<0) errexit(1, M)  
                       
#endif
