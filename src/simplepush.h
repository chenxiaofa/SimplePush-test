#ifndef SIMPLEPUSH_H_INCLUDED
#define SIMPLEPUSH_H_INCLUDED

#define VERSION 0.0.01

#define PROTNUMBER 80
#define MAX_FD 65536
#define MAX_EPOLL 65536
#define MAX_EPOLL_EVENTS 256
#define MAX_LISTEN_QUEUE_NUM 65536
#define MAX_READ_LENGTH 2048

#define SOCK_FD int

#define UN_INT16 unsigned int
#define UN_INT32 unsigned long
#define UN_INT8  unsigned char
#define INT16 int


#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>


#include "error.h"
#include "linklist.h"
#include "queue.h"
#include "connectionpool.h"
#include "list.h"
#include "sock.h"
#include "http.h"
#include "push.h"
#include "workers.h"

#include "application.h"
#include "session.h"


uint16_t count;


#endif // SIMPLEPUSH_H_INCLUDED

