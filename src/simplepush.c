#include "simplepush.h"

#include <sys/resource.h>
#define THREAD_NUM 10

queue_t main_socket_even_queue;

link_list_t socket_fd_list,push_list;

workers_t workers;



int main()
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



    char push_txt[256];

    uint16_t fd = 0;

    char input;


    queue_init(&main_socket_even_queue);



    link_list_init(&socket_fd_list);
    link_list_init(&push_list);


    connectionpool_init();

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
            push_to_connections(make_http_jsonp_response(push_txt));
        }
        else if(input == 'c')
        {
            printf("count:%d\r\n",count);
        }
        else if(input == 'd')
        {
            scanf("%d",&fd);
            link_delete_node(&socket_fd_list,fd);
        }
    }

}
