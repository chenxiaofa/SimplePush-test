#include "simplepush.h"

#ifndef QUEUE_INCULDED
#define QUEUE_INCULDED

#define QUEUE_FAILED -1
#define QUEUE_OK 0
#define QUEUE_BUSY 1
#define QUEUE_EMPTY 2

#define NewQNode() (queue_node_t*)malloc(sizeof(queue_node_t))

typedef struct QNode{
  INT16        data;
  struct QNode    *next;
}queue_node_t;

typedef struct Queue{
    queue_node_t* front;
    queue_node_t* rear;
    pthread_rwlock_t lock;
    pthread_mutex_t mutex;
    pthread_cond_t cond1;

}queue_t;

INT16 queue_init(queue_t *queue);
INT16 en_queue(queue_t *queue,INT16 data);
INT16 de_queue(queue_t *queue,INT16 *data);
void print_queue(queue_t *queue);
INT16 en_queue_block(queue_t *queue,UN_INT16 data);

#endif
