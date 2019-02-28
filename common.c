/*************************************************************************
	> File Name: common.c
	> Author: 
	> Mail: 
	> Created Time: 2019年02月23日 星期六 11时08分43秒
 ************************************************************************/

#include<stdio.h>
#include "common.h"
int socket_create(int getport){
   // printf("进来了 %d\n",getport);
    int port,server_listen;
    port = getport;
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);

    if((server_listen=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("create socket:");
        return -1;    
    }
      if(bind(server_listen,(struct sockaddr * )&server_addr,sizeof(server_addr))<0){
        perror("bind:");
        close(server_listen);
        return -1;
    }
    if(listen(server_listen,20)<0){
        perror("listen:");
        close(server_listen);
        return -1;
    }
   // printf("准备返回了!\n");
    return server_listen; 
}
int socket_connect(int getport ,char *host){
  /*  struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    client_addr.sin_family=AF_INET;
    client_addr.sin_addr.s_addr=htons(INADDR_ANY);
    client_addr.sin_port=htons(0);*/
   // printf("connect    %d %s",getport,host);
    int client_socketfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (client_socketfd < 0) { 
     //   printf("Create Socket Failed!\n");
        return 0;  
    }
    int port_server;
  //  port_server=atoi(getport);
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(getport);
    server_addr.sin_addr.s_addr=inet_addr(host);
    if(connect(client_socketfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr))==-1){//连接方法，传入句柄，目标地址和大小 
       // printf("connect failed");//失败时可以打印errno 
    } else{
       // printf("cheng gong");
        return client_socketfd;
    }
}

int get_conf_value(char* pathname,char* key_name,char *value){
    FILE *fp;
    char Strline[1024];
    char *line=NULL;
    size_t len=0;
    size_t read=0;
    if((fp=fopen(pathname,"r"))==NULL){
        printf("ERROR");
        exit(1);
    }else{
        while((read=getline(&line,&len,fp))!= -1){
            char *p=strstr(line,key_name);
          //  printf("%d\n",(int)read);
          //  printf("%s",line);
            if(p!=NULL){
            //    printf("zhaodao %s",p);
                int tmp=strlen(key_name);
          //      printf("tmp=%d\n",tmp);
                if(line[tmp]=='='){
                    strncpy(value,&line[tmp+1],(int)read-tmp-1);
                    tmp=strlen(value);
                    *(value + tmp - 1)='\0';
    //                printf("daozhele %s\n",value);
                    break;
                }
            }
        }
    }
    fclose(fp);
    return 0;
}
