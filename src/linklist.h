#include "simplepush.h"

#ifndef LINKTABLE_H_INCLUDED
#define LINKTABLE_H_INCLUDED

#define NEWLNODE() (link_node_t*)malloc(sizeof(link_node_t))
typedef struct link_node{
    void* data;
    struct link_node* next;
}link_node_t;

typedef struct l_list{
    link_node_t* head;
    pthread_mutex_t mutex;
}link_list_t;

typedef void (*traversal_func)(void* data,char* extra);

void link_list_init(link_list_t* link);
void link_insert_node(link_list_t* link,void* );
void link_delete_node(link_list_t* link,void* );

void print_list(link_list_t* link);
void traversal(link_list_t *link ,traversal_func func,char* extra);
typedef link_list_t push_list_t;
#endif // LINKTABLE_H_INCLUDED
