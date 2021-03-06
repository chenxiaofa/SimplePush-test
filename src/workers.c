#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>

#include "simplepush.h"

//task queue
static queue_t* _tasks_queue = NULL;

//fd list
static link_list_t* _fd_list = NULL;




void  worker_set_task_queue(queue_t* queue){
    _tasks_queue = queue;
}
void worker_set_fd_list(link_list_t *list){
    _fd_list = list;
}

INT16 workers_init(workers_t *workers,INT16 max_thread){
	INT16 i_loop = 0;
	worker_t *curr_worker = NULL;
	if(workers == NULL || max_thread < 1)
		return WORKERS_INIT_FAILED;

	workers->workers = (worker_t**)malloc(sizeof(worker_t*)*max_thread);
	workers->workers_num = max_thread;
	for(i_loop=0;i_loop<max_thread;i_loop++){

		curr_worker = workers->workers[i_loop];
		curr_worker = (worker_t*)malloc(sizeof(workers_t));

		pthread_create(&curr_worker->thread_id,NULL,work_function,(void*)i_loop);
	}
	return 1;
}
void close_socket_fd(SOCK_FD fd){
    close_socket(fd);
    connection_t* conn_data = get_connection_sctuct(fd);
    conn_data->offset=0;
}

void* work_function(void* arg){

	INT16 sock_fd = 0;
	INT16 status      = 0;

	INT16 read_length = 0;
	//printf("worker thread is created id->%u,arg->%d \r\n",pthread_self(),(INT16)arg);

	while( (status = de_queue(_tasks_queue,&sock_fd)) != QUEUE_FAILED){
		if(status==QUEUE_OK){
            connection_t* conn_data = get_connection_sctuct(sock_fd);
			read_length = read(sock_fd, conn_data->buff, MAX_READ_LENGTH);
			//printf("thread(id:%u) is reading data length=%d\r\n",(long)pthread_self(),read_length);
			if(read_length < 0 ){
			    printf("read socket error: %s(errno: %d)\n",strerror(errno),errno);
			    remove_from_push_list(_fd_list,sock_fd);
			    close_socket_fd(sock_fd);

			}else if(read_length == 0){
                printf("sock_fd:%d exitting \r\n",sock_fd);
                remove_from_push_list(_fd_list,sock_fd);
                close_socket_fd(sock_fd);
            }else{
                http_parse(conn_data->buff,read_length,conn_data);

            }

		}
	}
	return NULL;
}
