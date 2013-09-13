#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include "main.h"
#include "workers.h"
#include "Queue.h"
#include "sock.h"
#include "linktable.h"

//task queue
Queue *_tasks_queue = NULL;

//fd list
l_list *_fd_list = NULL;

void set_fd_list(l_list *list){
    _fd_list = list;
}

INT16 workers_init(WORKERS *workers,INT16 max_thread){
	INT16 i_loop = 0;
	WORKER *curr_worker = NULL;
	if(workers == NULL || max_thread < 1)
		return WORKERS_INIT_FAILED;

	if(workers->tasks_queue == NULL)
		return WORKERS_INIT_FAILED;

	_tasks_queue = workers->tasks_queue;

	workers->workers = (WORKER**)malloc(sizeof(WORKER*)*max_thread);
	workers->workers_num = max_thread;
	for(i_loop=0;i_loop<max_thread;i_loop++){

		curr_worker = workers->workers[i_loop];
		curr_worker = (WORKER*)malloc(sizeof(WORKER));

		pthread_create(&curr_worker->thread_id,NULL,work_function,(void*)i_loop);
	}
}


void* work_function(void* arg){
    INT16 i = 0;
	INT16 sock_fd = 0;
	INT16 status      = 0;
	char  buff[MAX_READ_LENGTH];
	INT16 read_length = 0;
	//printf("worker thread is created id->%u,arg->%d \r\n",pthread_self(),(INT16)arg);

	while( (status = de_queue(_tasks_queue,&sock_fd)) != QUEUE_FAILED){
		if(status==QUEUE_OK){

			read_length = read(sock_fd, buff, MAX_READ_LENGTH);
			//printf("thread(id:%u) is reading data length=%d\r\n",(long)pthread_self(),read_length);
			if(read_length < 0 ){
			    link_delete_node(_fd_list,sock_fd);
			    --count;

			}else if(read_length == 0){
                printf("sock_fd:%d exitting \r\n",sock_fd);
                link_delete_node(_fd_list,sock_fd);
                close_socket(sock_fd);
                --count;
            }else if(buff[0]=='r'){
                link_inset_node(_fd_list,sock_fd);
                write(sock_fd,"registered\r\n",12);
            }else if(buff[0]=='q'){
                link_delete_node(_fd_list,sock_fd);
                write(sock_fd,"Unregistered\r\n",12);
            }else{
                buff[read_length] = '\0';
                printf("%s",buff);
            }

		}
	}
	printf("thread(id:%u) dead \r\n",(long)pthread_self());
	return NULL;
}


void push(SOCK_FD fd,char* extra){
    write(fd,extra,strlen(extra));
}

