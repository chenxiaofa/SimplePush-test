#include "simplepush.h"

void link_list_init(link_list_t* link)
{
    link->head        =  NEWLNODE();
    link->head->fd    =  0;
    link->head->next  =  NULL;
    pthread_mutex_init(&(link->mutex),NULL);
}

void link_inset_node(link_list_t* link,SOCK_FD sock_fd)
{

    if(link==NULL)
        return;

    pthread_mutex_lock(&(link->mutex));

    link_node_t* p  = NULL;
    link_node_t* np = NULL;

    p = link->head;

    while(p->next)
    {

        if(p->next->fd==sock_fd)
        {
            pthread_mutex_unlock(&(link->mutex));
            return;
        }


        if(p->next->fd > sock_fd)
            break;
        p = p->next;
    }


    np = NEWLNODE();
    if(np==NULL){
        FAIL_WITH_ERROR("malloc faild linklist.c link_inset_node");
    }
    np->fd   = sock_fd;

    np->next = p->next;
    p->next  = np;

    pthread_mutex_unlock(&(link->mutex));


}
void link_delete_node(link_list_t* link,SOCK_FD sock_fd)
{

    if(link==NULL)

        return;

    pthread_mutex_lock(&(link->mutex));

    link_node_t* p  = NULL;
    link_node_t* np = NULL;

    p = link->head;


    while(p->next)
    {
        if(p->next->fd == sock_fd)
        {


            np = p->next;

            p->next = np->next;

            np->fd   = 0;
            np->next = NULL;
            free(np);
            break;
        }
        p = p->next;

    }

    pthread_mutex_unlock(&(link->mutex));

}

void traversal(link_list_t *link ,traversal_func func,char* extra)
{
    INT16 count=0;
    link_node_t* p  = NULL;
    p = link->head->next;

    while(p)
    {
        count++;
        (*func)(p->fd,extra);
        p = p->next;
    }
    printf("traversal num:%d\r\n",count);
}

void print_list(link_list_t *link)
{

    link_node_t* p  = NULL;
    p = link->head->next;

    while(p)
    {
        printf("fd:%d \r\n",p->fd);
        p = p->next;
    }

}
