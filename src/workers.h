#include "simplepush.h"

#ifndef WORKERS_INCULDED
#define WORKERS_INCULDED 1

#define SOCKET_BUFF_SIZE 256

typedef struct worker_t{
	pthread_t thread_id;
	UN_INT16 curr_work;
}worker_t;

typedef struct workers_t{
	worker_t	  **workers;
	UN_INT16  workers_num;
	queue_t  *tasks_queue;
}workers_t;


#define WORKERS_INIT_FAILED -1;

INT16 workers_init(workers_t *workers,INT16 max_thread);
void* work_function(void* arg);
void  worker_set_fd_list(link_list_t* list);
void  worker_set_task_queue(queue_t* queue);
#endif
