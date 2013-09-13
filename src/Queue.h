#ifndef QUEUE_INCULDED
#define QUEUE_INCULDED 1

#include <pthread.h>
#include <errno.h>
#include "main.h"
#define QUEUE_FAILED -1
#define QUEUE_OK 0
#define QUEUE_BUSY 1
#define QUEUE_EMPTY 2

#define NewQNode() (QNodePtr)malloc(sizeof(struct QNode))
typedef struct QNode{
  INT16        data;
  struct QNode    *next;
}QNode , * QNodePtr;

typedef struct Queue{
    QNodePtr front;
    QNodePtr rear;
    pthread_rwlock_t lock;
    pthread_mutex_t mutex;
    pthread_cond_t cond1;

}Queue;
typedef struct QData{

}QData;

INT16 init_queue(Queue *queue);
INT16 en_queue(Queue *queue,INT16 data);
INT16 de_queue(Queue *queue,INT16 *data);
void print_queue(Queue *queue);
INT16 en_queue_block(Queue *queue,UN_INT16 data);

#endif
