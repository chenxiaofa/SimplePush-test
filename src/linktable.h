#ifndef LINKTABLE_H_INCLUDED
#define LINKTABLE_H_INCLUDED

#include "main.h"
#include "pthread.h"
#define NEWLNODE() (l_node_ptr)malloc(sizeof(l_node))
typedef struct l_node{

    SOCK_FD fd;
    struct l_node* next;

}l_node,* l_node_ptr;

typedef struct l_list{

    l_node_ptr head;
    pthread_mutex_t mutex;


}l_list;
typedef void (*traversal_func)(INT16 fd,char* extra);

void link_list_init(l_list* link);
void link_inset_node(l_list* link,SOCK_FD sock_fd);
void link_delete_node(l_list* link,SOCK_FD sock_fd);

void print_list(l_list* link);
void traversal(l_list *link ,traversal_func func,char* extra);

#endif // LINKTABLE_H_INCLUDED
