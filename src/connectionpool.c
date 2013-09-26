#include "simplepush.h"

static connection_t connections[MAX_FD];
/*
uint16_t connection_scruct_init(connection_t* connection){

    if(connection==NULL)
        return 0;
    if(connection->buff==NULL){
        connection->buff = (void*)malloc(SOCKET_BUFF_SIZE);
        if(connection->buff==NULL){
            FAIL_WITH_ERROR("malloc failed connectionpool.c connection_scruct_init");
        }
    }

    time(&(connection->last_act));
    connection->offset = 0;

}
*/
inline void connection_lock(connection_t* connection){
    pthread_mutex_lock(&(connection->mutex));
}
inline void connection_unlock(connection_t* connection){
    pthread_mutex_unlock(&(connection->mutex));
}


void connectionpool_init(){
    int i = 0;
    for(i=0;i<MAX_FD;i++){
        pthread_mutex_init(&(connections[i].mutex),NULL);
        connections[i].fd = i;
    }


}
connection_t* get_connection_sctuct(uint16_t fd){

    connection_t* curr_p = (connections+fd);

    return curr_p;

}
