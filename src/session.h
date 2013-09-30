#include "simplepush.h"

#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED

#define NEW_SESSION() (session_t*)malloc(sizeof(session_t))

typedef struct session_t
{
    uint16_t session_id;
    char app_id[8];
    uint16_t fd;
    struct session_t* next;
}session_t;

typedef struct free_session_list_t{



}free_session_list_t;

void session_init();
session_t* get_session(uint16_t sid);
session_t* get_free_session();
void release_session(session_t* s);
#endif // SESSION_H_INCLUDED
