#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include "stdio.h"
#include "stdlib.h"

#define LOG_ERROR(msg) \
fprintf(stderr, "raphters: %s\n", msg)

#define FAIL_WITH_ERROR(msg) \
fprintf(stderr, "raphters, fatal: %s\n", msg); \
exit(1)


#endif // ERROR_H_INCLUDED
