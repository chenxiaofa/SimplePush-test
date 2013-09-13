#ifndef WORKERS_INCULDED
#define WORKERS_INCULDED 1


#include <pthread.h>
#include "main.h"
#include "Queue.h"
#include "linktable.h"
typedef struct WORKER{
	pthread_t thread_id;
	UN_INT16 curr_work;
}WORKER;

typedef struct WORKERS{
	WORKER	  **workers;
	UN_INT16  workers_num;
	Queue	  *tasks_queue;
}WORKERS;

#define WORKERS_INIT_FAILED -1;

INT16 workers_init(WORKERS *workers,INT16 max_thread);
void* work_function(void* arg);
void set_fd_list(l_list* list);
void push(SOCK_FD fd,char* extra);
#endif
