#include "simplepush.h"

#include <sys/resource.h>
#define THREAD_NUM 10

queue_t main_socket_even_queue;

link_list_t socket_fd_list,push_list;

workers_t workers;



int main()
{
    {
        struct rlimit rt;
        rt.rlim_max = rt.rlim_cur = MAX_FD;
        if (setrlimit(RLIMIT_NOFILE, &rt) == -1)
        {
            perror("setrlimit");
            exit(1);
        }
        else
        {
            printf("set system resource success\n");
        }
   }//set max fd number


    char push_txt[256];

    uint16_t fd = 0;

    char input;


    queue_init(&main_socket_even_queue);

    init_app("WP00001");
    init_app("WP00002");

    link_list_init(&socket_fd_list);
    link_list_init(&push_list);

    session_init();

    connectionpool_init();
    http_init();
    worker_set_task_queue(&main_socket_even_queue);
    worker_set_fd_list(&socket_fd_list);

    workers_init(&workers,THREAD_NUM);

    sock_set_queue(&main_socket_even_queue);
    sock_init();


    while(1)
    {
        scanf("%c",&input);
        if(input=='s')
        {
            show_fd_list();
        }
        else if(input=='q')
        {
            printf("exit\r\n");
            stop_listen();
            return 0;
        }
        else if(input == 'p')
        {
            gets(push_txt);
            printf("sid:");
            scanf("%d",&input);
            gets(push_txt);
            printf("text:");
            gets(push_txt);

            {
                session_t* s = get_session(input);
                if(s==NULL)
                    printf("invalid sid\r\n");
                else
                {
                    if(s->fd == 0)
                        printf("zero fd\r\n");
                    else
                    {
                        printf("%d=>%s\r\n",s->fd,push_txt);
                        push_to_fd_http(s->fd,push_txt);
                    }
                }

            }

            //push_to_connections(make_http_jsonp_response(push_txt));
        }
        else if(input == 'c')
        {
            scanf("%d",&fd);
            close_socket(fd);
        }
        else if(input == 'd')
        {
            scanf("%d",&fd);
            link_delete_node(&socket_fd_list,(void*)fd);
        }else if(input == 't')
        {
            list_t list;
            list.step = 1000;
            list_init(&list);

            list_increase(&list);

            {
                int loop = 0;
                for(;loop<2000;loop++)
                {
                    list_set(&list,loop,(void*)loop);
                    printf("list[%d]=>%p\r\n",loop,list_get(&list,loop));
                }
            }

        }
    }

}
