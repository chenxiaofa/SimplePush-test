#include "simplepush.h"

queue_t *queue = NULL;

INT16 queue_init(queue_t *queue){
	if(queue != NULL){
		queue->front = queue->rear  =  NewQNode();
		pthread_mutex_init(&(queue->mutex),NULL);
        pthread_cond_init(&(queue->cond1),NULL);
		if(queue->front == NULL){
			return QUEUE_FAILED;
		}
		queue = queue;
		return QUEUE_OK;
	}else{
		return QUEUE_FAILED;
	}
}

INT16 en_queue(queue_t *queue,INT16 data){
  	INT16 results = QUEUE_OK;
	if(queue != NULL){

		pthread_mutex_lock(&(queue->mutex));


		queue_node_t* p = NewQNode();
		if(p == NULL) {
			pthread_mutex_unlock(&(queue->mutex));

			results = QUEUE_FAILED;
		}else{
			p->data  = data;
			p->next  = NULL;
			queue->rear->next = p;
			queue->rear = p;

            pthread_cond_broadcast(&(queue->cond1));

			results = QUEUE_OK;
		}
		pthread_mutex_unlock(&(queue->mutex));
	}else{
		results = QUEUE_FAILED;
	}
	return results;
}
INT16 de_queue(queue_t *queue,INT16 *data){
	INT16 results = QUEUE_OK;
	if(queue != NULL){

		pthread_mutex_lock(&(queue->mutex));

		while(queue->front->next == NULL)
            pthread_cond_wait(&(queue->cond1),&(queue->mutex));

		if(queue->front->next != NULL){
			queue_node_t* p = queue->front->next;
			queue->front->next = p->next;
			*data = p->data;
			if(queue->rear == p)queue->rear = queue->front;
			free(p);
			results = QUEUE_OK;
		}else{
			results = QUEUE_EMPTY;
		}

		pthread_mutex_unlock(&(queue->mutex));
	}else{
		results = QUEUE_FAILED;
	}

	return results;
}


void print_queue(queue_t *queue){
if(queue != NULL){
queue_node_t* p = queue->front->next;
while(p != NULL){
printf("%d\r\n",p->data);
p = p->next;
}
  }
}
