#include "simplepush.h"

#ifndef PUSH_H_INCLUDED
#define PUSH_H_INCLUDED


uint16_t add_to_push_list(link_list_t* push_list,SOCK_FD fd);

uint16_t remove_from_push_list(link_list_t* push_list,SOCK_FD fd);

void push_to_fd(SOCK_FD fd,char* extra);

void push_to_list(link_list_t* push_list,char* extra);

#endif // PUSH_H_INCLUDED
