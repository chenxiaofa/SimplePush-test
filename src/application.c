#include "simplepush.h"

#define NEW_APP_NODE (application_t*)malloc(sizeof(application_t))
#define C2N(C) C-'0'
#define APP(APP_ID) app_list[C2N(APP_ID[2])][C2N(APP_ID[3])][C2N(APP_ID[4])][C2N(APP_ID[5])][C2N(APP_ID[6])]



static application_t* app_list[10][10][10][10][10];

void init_app(char* app_id)
{
    APP(app_id) = NEW_APP_NODE;

    memcpy(APP(app_id)->app_id,app_id,8);

    //LOGGING(app_list[0][0][0][0][1]->app_id);
    //LOGGING(app_list[0][0][0][0][2]->app_id);
}

uint8_t client_regiest(char* app_id,connection_t* conn)
{
    int i = 0;

    if(app_id[0] != 'W' || app_id[1] != 'P' || strlen(app_id) != 7)
        return INVALID_APP_ID;

    for(i=2;i<7;i++)
        if(app_id[i]<'0' || app_id[i]>'9')
            return INVALID_APP_ID;

    if(APP(app_id)==NULL)
        return INVALID_APP_ID;

    link_insert_node(&(APP(app_id)->client_list),conn);
}
