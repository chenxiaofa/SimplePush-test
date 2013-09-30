#include "simplepush.h"

uint16_t add_to_push_list(push_list_t* push_list,SOCK_FD fd){
    link_insert_node(push_list,(void*)fd);
    return 0;
}
uint16_t remove_from_push_list(push_list_t* push_list,SOCK_FD fd){
    link_delete_node(push_list,(void*)fd);
    return 0;
}

inline void push_to_fd(SOCK_FD fd,char* extra){
    //printf("sending\r\n");
    write(fd,extra,strlen(extra));
    //printf("sent\r\n");
}
void push_to_fd_traversal(void* fd ,char* data){
    push_to_fd((SOCK_FD)fd,data);
}

void push_to_list(push_list_t* push_list,char* extra){
    traversal(push_list,push_to_fd_traversal,extra);
}

void push_to_fd_http(SOCK_FD fd,char* content)
{
    char response[1024];
    push_to_fd(fd,make_jsonp_response(response,content));

}
