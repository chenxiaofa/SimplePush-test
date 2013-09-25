#include "simplepush.h"

#ifndef SOCK_H_INCLUDED
#define SOCK_H_INCLUDED

#define SOCK_OK 0
#define SOCK_INIT_FAILED -1


#define SOCK_LISTENING 1
#define SOCK_LISTEN_STOP 0

#define SOCK_EPOLL_INIT_FAILED -1

uint16_t sock_init();
void* listen_thread(void *arg);
void* listen_thread_epoll(void* arg);
void  sock_set_queue(queue_t *queue);
void  stop_listen();
void  close_socket(SOCK_FD fd);
void  show_fd_list(void);
void  push_to_connections(void* data);
void  sock_set_connection_poll(connection_t* connection_poll);


#endif // SOCK_H_INCLUDED
