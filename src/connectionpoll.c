#include "simplepush.h"

static connection_t connections[MAX_FD];

uint16_t connection_scruct_init(connection_t* connection){

    if(connection==NULL)
        return 0;
    if(connection->buff==NULL)
        connection->buff = (void*)malloc(SOCKET_BUFF_SIZE);
    time(&(connection->last_act));
    connection->offset = 0;

}

inline void connection_lock(connection_t* connection){
    pthread_mutex_lock(&(connection->mutex));
}
inline void connection_unlock(connection_t* connection){
    pthread_mutex_unlock(&(connection->mutex));
}


void connectionpool_init(){
       memset(connections,0,sizeof(connection_t)*MAX_FD);
}
connection_t* get_connection_sctuct(uint16_t fd){

    connection_t* curr_p = (connections+fd);

    connection_lock(curr_p);
    if(curr_p->fd==0)
        connection_scruct_init(curr_p);
    curr_p->fd = fd;
    connection_unlock(curr_p);

    return curr_p;

}
