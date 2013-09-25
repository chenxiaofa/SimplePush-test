#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include "stdio.h"
#include "stdlib.h"

#define DEBUG(tid,msg)\
fprintf(stderr,"thread(%u):%s\n",tid,msg)

#define LOGGING(msg)\
fprintf(stderr, "simplePush: %s\n", msg)

#define LOG_ERROR(msg) \
fprintf(stderr, "simplePush: %s\n", msg)

#define FAIL_WITH_ERROR(msg) \
fprintf(stderr, "simplePush, fatal: %s\n", msg); \
exit(1)


#endif // ERROR_H_INCLUDED
