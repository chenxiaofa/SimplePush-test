#include "simplepush.h"


int total = 0;
int deep  = 0;
void** root = NULL;
void list_init(list_t* list)
{
    list->total = list->step;
    list->deep = 0;
    APPLY_SPACE(list->root,list->step);
}

void list_increase(list_t* list)
{
    {
        long t = 0;
        list->total += list->step;
        POW(t,list->step,list->deep+1);
        if(list->total > t)
        {
            void** p = list->root;
            APPLY_SPACE(list->root,list->step);
            list->root[0] = p;
            list->deep++;
        }
    }
    {
        void** p = list->root;
        int i = list->deep;
        int pos = 0;
        long t = 0;
        for(;i>0;i--)
        {
            void* tp = NULL;
            POW(t,list->step,i);
            pos=((list->total-1)/t)%list->step;
            if(p[pos]==NULL)
                APPLY_SPACE(p[pos],list->step);
            p = p[pos];
        }
    }
}
void* list_get(list_t* list,int pos)
{
    void** temp_list = list->root;/////////////
    if(!(pos < list->total)){return NULL;}
    LIST(temp_list,pos);
    return temp_list[pos%list->step];
}
void* list_set(list_t* list,int pos,void* data)
{
    void** temp_list = list->root;
    if(!(pos < list->total)){return NULL;}

    LIST(temp_list,pos);
    temp_list[pos%list->step] = data;
}

