#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "simplepush.h"

#define STEP 1000

#define SET_SERO(p,n) memset(p,0,n*sizeof(void*))
#define APPLY_SPACE(p,n) \
{\
p = (void**)malloc(n*sizeof(void*));\
SET_SERO(p,n);\
}

#define POW(result,x,y) \
{\
    int loop=0;\
    result=1;\
    for(loop=0;loop<y;loop++){\
    result *= x;\
    }\
}\

#define LIST(list,pos) \
{\
    int i=deep;\
    long a;\
    for(;i>0;i--){\
        POW(a,STEP,i);\
        list = list[(pos/a)%STEP];\
    }\
}

typedef struct list_t
{
    void** root;
    long total;
    int deep;
    int step;
} list_t;


void list_init(list_t* list);
void list_increase(list_t* list);
void* list_get(list_t* list,int pos);
void* list_set(list_t* list,int pos,void* data);
#endif //LIST_H_INCLUDED
