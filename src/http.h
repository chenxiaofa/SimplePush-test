#include "simplepush.h"

#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

enum ACTIONS{
    ACTION_REGIEST,
    ACTION_POLLING
};

#define PARAM_ACTION "action"
#define PARAM_APPID "app_id"

char* int_to_str(int len);
char* make_http_jsonp_response(char* data);
void http_init();
void http_parse(char* data,size_t data_length,connection_t* conn);
char* make_jsonp_response(char* buff,char* data);
#endif // HTTP_H_INCLUDED
