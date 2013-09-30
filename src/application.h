#include "simplepush.h"

#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED


enum APP_ERRS{
    INVALID_APP_ID = -1
};

typedef struct application_t{
    char        app_id[8];
    link_list_t    client_list;
}application_t;

void init_app(char* app_id);
uint8_t client_regiest(char* app_id,connection_t* conn);

#endif // APPLICATION_H_INCLUDED
