#include "simplepush.h"
#include "http_parser.h"

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

char* make_jsonp_response(char* buff,char* data)
{
    char* json_front = "jCallback({\"data\":\"";
    char* json_rear  = "\"});";
    char js_data[256];
    buff[0] = 0;
    js_data[0]     = 0;

    strcat(js_data,json_front);
    strcat(js_data,data);
    strcat(js_data,json_rear);

    strcat(buff,response_status);
    strcat(buff,response_type);
    strcat(buff,response_allow_origin);
    strcat(buff,response_length);
    strcat(buff,int_to_str(strlen(js_data)));
    strcat(buff,response_double_crlf);
    strcat(buff,js_data);
    return buff;
}

static http_parser_settings settings;

/*
int on_message_begin(http_parser* _)
{
    (void)_;
    printf("\n***MESSAGE BEGIN***\n\n");
    return 0;
}
*/
int on_headers_complete(http_parser* _)
{
    connection_t* conn = (connection_t*)_->data;
    printf("\n***HEADERS COMPLETE***\n\n");
    printf("app_id:%s \r\n",conn->app_id);
    printf("action:%d \r\n",conn->action);

    printf("ACTION_REGIEST:%d\r\n",ACTION_REGIEST);
    if(conn->action == ACTION_REGIEST)
        push_to_fd(conn->fd,make_jsonp_response(conn->buff,"LOGIN,OK"));
    return 0;
}
/*
int on_message_complete(http_parser* _)
{
    (void)_;
    printf("\n***MESSAGE COMPLETE***\n\n");
    return 0;
}
*/
inline char _strcmp(const char* a,const char* b){

    int offset = 0;

    //printf("_strcmp a=>%s\r\n",a);
    //printf("_strcmp b=>%s\r\n",b);

    while(*(a+offset)!=0 && *(b+offset)!=0)
    {
        if(*(a+offset) != *(b+offset))
            return 0;
        offset++;
    }
    return 1;
}
static char* find_param(const char* uri,size_t uri_length,const char* param,size_t* value_length)
{
    size_t offset = 0;
    while(offset < uri_length)
    {
        if(_strcmp((uri+offset),param))
        {
            offset += strlen(param)+1;
            *value_length = 0;
            while((offset+*value_length) < uri_length && *(uri+offset+*value_length)!='\&')
                (*value_length)++;
            return (uri+offset);
        }
        offset++;
    }
    return NULL;
}
int on_url(http_parser* _, const char* at, size_t uri_length)
{
    connection_t* conn = (connection_t*)_->data;
    size_t value_length = 0;
    char*  param_start  = NULL;


    param_start = find_param(at,uri_length,PARAM_ACTION,&value_length);
    memcpy(&(conn->action),param_start,1);

    conn->action -= '0';

    param_start = find_param(at,uri_length,PARAM_APPID,&value_length);
    memcpy(&(conn->app_id),param_start,value_length);

    conn->app_id[value_length] = '\0';

    return 0;
}

int on_header_field(http_parser* _, const char* at, size_t length)
{
    (void)_;
    printf("Header field: %.*s\n", (int)length, at);
    return 0;
}

int on_header_value(http_parser* _, const char* at, size_t length)
{
    (void)_;
    printf("Header value: %.*s\n", (int)length, at);
    return 0;
}

int on_body(http_parser* _, const char* at, size_t length)
{
    (void)_;
    printf("Body: %.*s\n", (int)length, at);
    return 0;

}

void http_parse(char* data,size_t data_length,connection_t* conn)
{

    conn->parser.data = conn;
    http_parser_init(&(conn->parser), HTTP_REQUEST);
    size_t nparsed = http_parser_execute(&(conn->parser), &settings, data, data_length);

    if (nparsed != (size_t)data_length)
    {
        fprintf(stderr,
                "Error: %s (%s)\n",
                http_errno_description(HTTP_PARSER_ERRNO(&conn->parser)),
                http_errno_name(HTTP_PARSER_ERRNO(&conn->parser)));
    }
}


void http_init()
{
    memset(&settings, 0, sizeof(settings));
    //settings.on_message_begin = on_message_begin;
    settings.on_url = on_url;
    //settings.on_header_field = on_header_field;
    //settings.on_header_value = on_header_value;
    settings.on_headers_complete = on_headers_complete;
    //settings.on_body = on_body;
    //settings.on_message_complete = on_message_complete;
}
