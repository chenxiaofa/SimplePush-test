#include "simplepush.h"

#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

char* int_to_str(int len);
char* make_http_jsonp_response(char* data);
void http_init();
void http_parse(char* data,size_t data_length);
#endif // HTTP_H_INCLUDED
