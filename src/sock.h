#include "main.h"
#include "Queue.h"

#define SOCK_OK 0
#define SOCK_INIT_FAILED -1

#define PROTNUMBER 6666
#define MAX_EPOLL 1024

#define MAX_EPOLL_EVENTS 64

#define MAX_LISTEN_QUEUE_NUM 1024
#define MAX_READ_LENGTH 1024

#define SOCK_LISTENING 1
#define SOCK_LISTEN_STOP 0


#define SOCK_EPOLL_INIT_FAILED -1

INT16 sock_init();
void* listen_thread(void *arg);
void* listen_thread_epoll(void* arg);
void set_queue(Queue *queue);
void stop_listen();
void close_socket(SOCK_FD fd);
