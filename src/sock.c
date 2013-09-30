#include "simplepush.h"

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>


//listening thread id
pthread_t listen_thread_id = 0;

//epolling thread id
pthread_t epoll_thread_id = 0;


//task queue
queue_t *_sock_task_queue = NULL;

// fd for socket listening
SOCK_FD listen_fd=-1;

//flag for listening control
INT16 listening = 0;

//epoll running
INT16 epolling  = 0;

// fd for epoll
SOCK_FD epoll_fd = 0;

//epoll event for fd add&remove
struct epoll_event ev;

//mutex for socket
pthread_mutex_t _socket_mutex;

//socket fd list
static link_list_t _socket_fd_list;

static connection_t** _connection_pool;

void  sock_set_connection_poll(connection_t* connection_pool){
    _connection_pool = connection_pool;
}


static void _mutex_lock(void){
    pthread_mutex_lock(&_socket_mutex);
}
static void _mutex_unlock(void){
    pthread_mutex_unlock(&_socket_mutex);
}

void sock_set_queue(queue_t *queue){
	_sock_task_queue = queue;
}

static void add_to_fd_list(SOCK_FD fd){
    link_insert_node(&_socket_fd_list,(void*)fd);
}
static void remove_from_fd_list(SOCK_FD fd){
    link_delete_node(&_socket_fd_list,(void*)fd);
}
void show_fd_list(void){
    print_list(&_socket_fd_list);
}

uint16_t sock_init(){

    //init mutex
    pthread_mutex_init(&_socket_mutex,NULL);

    //init fd list
    link_list_init(&_socket_fd_list);

    //create thread to listen socket
	if(pthread_create(&listen_thread_id,NULL,listen_thread,NULL)){
        return SOCK_INIT_FAILED;
    }else{
        return SOCK_OK;
    }

}


void add_epoll_event(SOCK_FD fd){

    _mutex_lock();
    ev.data.fd=fd;
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&ev);
    count++;
    _mutex_unlock();


}
void remove_epoll_event(SOCK_FD fd){
    _mutex_lock();
    ev.data.fd=fd;
    epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,&ev);
    count--;
    _mutex_unlock();
}
void close_socket(SOCK_FD fd){

    connection_t* c = get_connection_sctuct(fd);
    session_t* s = get_session(c->sid);
    s->fd=0;
    close(fd);

    remove_from_fd_list(fd);
    remove_epoll_event(fd);

}
void* epoll_thread(void* arg){
	INT16 nfds,i;
	SOCK_FD conn_fd,sock_readable_fd;

	//struct for epoll
	struct epoll_event events[MAX_EPOLL_EVENTS];


	struct sockaddr_in clientaddr;


    epolling = SOCK_LISTENING;

	while(epolling) {
		//等待epoll事件的发生
		nfds=epoll_wait(epoll_fd,events,MAX_EPOLL_EVENTS,-1);

		//printf("epoll events num:%d\r\n",nfds);
		//处理所发生的所有事件
		for(i=0;i<nfds;++i){

			if(events[i].events&EPOLLIN){
				if ( (sock_readable_fd = events[i].data.fd) < 0)
                        continue;

				en_queue(_sock_task_queue,sock_readable_fd);
			} else if(events[i].events&EPOLLERR){
                printf("event EPOLLERR\r\n");


			} else if(events[i].events&EPOLLRDHUP){
                    close(events[i].data.fd);
                    --count;
                    printf("event EPOLLRDHUP\r\n");
			}
		}
	}
	return NULL;
}
void close_fd(void* fd,char* extra){
    close((SOCK_FD)fd);
}
void stop_listen(){
	if(listen_fd != -1){
		close(listen_fd);
		listening = SOCK_LISTEN_STOP;
		epolling  = SOCK_LISTEN_STOP;
		traversal(&_socket_fd_list,close_fd,(char*)NULL);
	}

}
INT16 init_epoll(struct epoll_event* ev){
    if(ev==NULL)
        return SOCK_EPOLL_INIT_FAILED;
    //create epoll
    epoll_fd=epoll_create(MAX_EPOLL);
    if(epoll_fd==-1){
        return SOCK_EPOLL_INIT_FAILED;
    }

    //set interesting events
    ev->events=EPOLLIN|EPOLLET|EPOLLERR|EPOLLRDHUP;

    //create thread to run epoll
    if(pthread_create(&epoll_thread_id,NULL,epoll_thread,NULL)){
        return SOCK_EPOLL_INIT_FAILED;
    }else{
        return SOCK_OK;
    }
}



void* listen_thread(void *arg){

	SOCK_FD conn_fd;
	struct sockaddr_in servaddr;
    INT16 portnumber = PROTNUMBER;

    //epoll event


    // create listening fd
	printf("listening thread created\r\n");
	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        	printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		return NULL;
    }

    // set listening info
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(portnumber);

	//bind port
	if( bind(listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
		close(listen_fd);
		exit(1);
	}

	//listen start
	if( listen(listen_fd, MAX_LISTEN_QUEUE_NUM) == -1){
		printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
		close(listen_fd);
		exit(1);
	}

    listening = SOCK_LISTENING;


    if( init_epoll(&ev) == SOCK_EPOLL_INIT_FAILED ){
        printf("Init epoll failed!\r\n");
        close(listen_fd);
        exit(1);
    }



	while(listening){
		if( (conn_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL)) == -1){
			printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
                stop_listen();
		    	continue;
		}else{

            add_to_fd_list(conn_fd);
            add_epoll_event(conn_fd);


		}

    }


	    close(listen_fd);
}

void push_to_connections(void* data){
    push_to_list(&_socket_fd_list,(char*)data);
}
