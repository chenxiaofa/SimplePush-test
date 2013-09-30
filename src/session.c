#include "simplepush.h"

#include "session.h"

static list_t* session_list;
static session_t* free_list;

void add_to_free(session_t* s);
void increace_session_list()
{
    int loop;
    list_increase(session_list);
    for(loop=session_list->total-session_list->step;loop<session_list->total;loop++)
    {
        session_t* p = NEW_SESSION();
        p->session_id = loop;
        add_to_free(p);
        p->session_id=loop;
        list_set(session_list,loop,p);
    }

}
void print_free_session_list()
{
    session_t* p = free_list->next;
    while(p)
    {
        printf("%d \r\n",p->session_id);
        p=p->next;
    }


}
void print_session_list()
{
    int loop=0;
    for(;loop<session_list->total;loop++)
    {
        session_t* p = list_get(session_list,loop);
        printf("%d\r\n",p->session_id);
    }

}
session_t* get_free_session()
{
    if(free_list->next==NULL)
        increace_session_list();
    session_t* p = free_list->next;
    free_list->next = p->next;
    p->next=NULL;
    return p;

}

void add_to_free(session_t* s)
{
    session_t* p = free_list->next;
    free_list->next = s;
    s->next = p;
}
void release_session(session_t* s)
{
    s->fd = 0;
    memset(s->app_id,0,8);
    add_to_free(s);
}
void session_init()
{
    session_list = (list_t*)malloc(sizeof(list_t));
    free_list    = (session_t*)malloc(sizeof(session_t));

    session_list->step = 10;
    free_list->next = NULL;
    list_init(session_list);

    {
        int loop =0;
        for(;loop<session_list->step;loop++)
        {
            session_t* p = NEW_SESSION();
            p->session_id = loop;
            add_to_free(p);
            p->session_id=loop;
            list_set(session_list,loop,p);
        }

    }
    //print_free_session_list();
    //session_t* p = get_free_session();
    //printf("///////////////////////////\n");
    //print_free_session_list();
    //release_session(p);
    //printf("///////////////////////////\n");
    //print_free_session_list();
    //print_session_list();
}
session_t* get_session(uint16_t sid)
{
    return (session_t*)list_get(session_list,sid);
}


