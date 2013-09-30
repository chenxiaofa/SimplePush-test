#include "simplepush.h"

#ifndef CONNCTIONPOLL_H_INCLUDED
#define CONNCTIONPOLL_H_INCLUDED
#include "http_parser.h"
typedef struct connection_t{

    uint16_t        fd;
    time_t          last_act;
    char            buff[MAX_READ_LENGTH];
    uint16_t        offset;
    char*           uri;
    char            app_id[8];
    uint8_t         action;
    uint16_t        sid;
    http_parser     parser;
    pthread_mutex_t mutex;

}connection_t;
uint16_t connection_scruct_init(connection_t* connection);

inline void connection_lock(connection_t* connection);
inline void connection_unlock(connection_t* connection);

void connectionpool_init();

connection_t* get_connection_sctuct(uint16_t fd);
#endif // CONNCTIONPOLL_H_INCLUDED
