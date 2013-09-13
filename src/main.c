#include <stdio.h>
#include "Queue.h"
#include "main.h"
#include "workers.h"
#include "sock.h"
#include "linktable.h"

void my_print(INT16 fd,char* extra){
    printf("my_print fd:%d\r\n",fd);
}

int main(){
  //task queue
  Queue mQueue;
  // fd list
  l_list fd_list;
  char push_txt[256];
  SOCK_FD fd = 0;
  char input;
  init_queue(&mQueue);
  link_list_init(&fd_list);

  WORKERS workers;
  set_fd_list(&fd_list);
  sock_init();
  workers.tasks_queue = &mQueue;
  workers_init(&workers,1);
  set_queue(&mQueue);

  while(1){
	scanf("%c",&input);
	if(input=='s'){
		print_queue(&mQueue);
		traversal(&fd_list,my_print,"");
	}else if(input=='q'){
		printf("exit\r\n");
		stop_listen();
		return 0;
	}else if(input == 'p'){
        gets(push_txt);
        traversal(&fd_list,push,push_txt);
	}else if(input == 'c'){
        printf("count:%d\r\n",count);
	}else if(input == 'd'){
        scanf("%d",&fd);
        link_delete_node(&fd_list,fd);
	}
  }

}
