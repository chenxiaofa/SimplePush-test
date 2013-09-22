#include "simplepush.h"

#ifndef CONNCTIONPOLL_H_INCLUDED
#define CONNCTIONPOLL_H_INCLUDED

typedef struct connection_t{

    uint16_t fd;
    time_t  last_act;
    char*   buff;
    uint16_t offset;
    pthread_mutex_t mutex;

}connection_t;
uint16_t connection_scruct_init(connection_t* connection);

inline void connection_lock(connection_t* connection);
inline void connection_unlock(connection_t* connection);

void connectionpool_init();

connection_t* get_connection_sctuct(uint16_t fd);
#endif // CONNCTIONPOLL_H_INCLUDED
