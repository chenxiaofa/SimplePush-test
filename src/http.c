#include "simplepush.h"


char* response_status       = "HTTP/1.1 200 OK\r\n";
char* response_type         = "Content-Type: application/x-javascript;charset=utf-8\r\n";
char* response_length       = "Content-Length:";
char* response_allow_origin = "Access-Control-Allow-Origin: *\r\n";
char* response_crlf         = "\r\n";
char* response_double_crlf  = "\r\n\r\n";

char  http_header[256];
char str_len[20];
char* int_to_str(int len)
{
    int p = 19;
    str_len[p]=0;
    do
    {
        str_len[--p] = '0' + (len % 10);
        len /= 10;
    }
    while(len != 0);
    return (char*)(str_len+p);
}

char* make_http_jsonp_response(char* data)
{
    char* json_front = "jCallback({\"data\":\"";
    char* json_rear  = "\"});";
    char js_data[256];
    http_header[0] = 0;
    js_data[0]     = 0;

    strcat(js_data,json_front);
    strcat(js_data,data);
    strcat(js_data,json_rear);

    strcat(http_header,response_status);
    strcat(http_header,response_type);
    strcat(http_header,response_allow_origin);
    strcat(http_header,response_length);
    strcat(http_header,int_to_str(strlen(js_data)));
    strcat(http_header,response_double_crlf);
    strcat(http_header,js_data);
    return http_header;
}
