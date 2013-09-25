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


static http_parser_settings settings;


int on_message_begin(http_parser* _) {
  (void)_;
  printf("\n***MESSAGE BEGIN***\n\n");
  return 0;
}

int on_headers_complete(http_parser* _) {
  (void)_;
  printf("\n***HEADERS COMPLETE***\n\n");
  return 0;
}

int on_message_complete(http_parser* _) {
    printf("%d\r\n",_->method);
  (void)_;
  printf("\n***MESSAGE COMPLETE***\n\n");
  return 0;
}

int on_url(http_parser* _, const char* at, size_t length) {
  (void)_;
  printf("Url: %.*s\n", (int)length, at);
  return 0;
}

int on_header_field(http_parser* _, const char* at, size_t length) {
  (void)_;
  printf("Header field: %.*s\n", (int)length, at);
  return 0;
}

int on_header_value(http_parser* _, const char* at, size_t length) {
  (void)_;
  printf("Header value: %.*s\n", (int)length, at);
  return 0;
}

int on_body(http_parser* _, const char* at, size_t length) {
  (void)_;
  printf("Body: %.*s\n", (int)length, at);
  return 0;



void http_parse(char* data,size_t data_length) {

  http_parser parser;
  http_parser_init(&parser, HTTP_REQUEST);
  size_t nparsed = http_parser_execute(&parser, &settings, data, data_length);

  if (nparsed != (size_t)data_length) {
    fprintf(stderr,
            "Error: %s (%s)\n",
            http_errno_description(HTTP_PARSER_ERRNO(&parser)),
            http_errno_name(HTTP_PARSER_ERRNO(&parser)));
  }
}
void http_init(){
  memset(&settings, 0, sizeof(settings));
  settings.on_message_begin = on_message_begin;
  settings.on_url = on_url;
  settings.on_header_field = on_header_field;
  settings.on_header_value = on_header_value;
  settings.on_headers_complete = on_headers_complete;
  settings.on_body = on_body;
  settings.on_message_complete = on_message_complete;
}
