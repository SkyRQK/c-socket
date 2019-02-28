/*************************************************************************
	> File Name: danliao.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年02月23日 星期六 17时04分08秒
 ************************************************************************/

#include<iostream>
#include "common.h"
#define CONFNAME  "common.conf"

using namespace std;

struct Message{
    char from[20];
    int flag;
    char message[1024];
}Message;

int main(int argc,char *argv[]){
    int sock_fd;
    char ip_addr[30]={0};
    char getport[10]={0};
    printf("%s\n",CONFNAME);
    get_conf_value(CONFNAME,"Server_Ip",ip_addr);
    get_conf_value(CONFNAME,"Server_Port",getport);
    int port=atoi(getport);
    char name[20]={0};
    get_conf_value(CONFNAME,"My_Name",name);
    cout<<CONFNAME<<endl;
    cout<<ip_addr<<"+"<<port<<"+"<<name<<endl;

    sock_fd=socket_connect(port,ip_addr);
    char bufSend[256] = {0};
    send(sock_fd, name, strlen(name), 0);
    pid_t fpid;
    fpid=fork();
    if(fpid<0){

    }else if(fpid==0){ 
        while(1){
            printf("Input a string: \n");
            scanf("%[^\n]",bufSend);
            getchar();
            send(sock_fd, bufSend, strlen(bufSend), 0);
            memset(bufSend,0,sizeof(bufSend));
        }
        close(sock_fd);//关闭socket
    
    }else{
        char log_file[20]={0};
        get_conf_value(CONFNAME,"Log_File",log_file);
        struct Message buff;
        while(recv(sock_fd,(char*)&buff,sizeof(buff),0)>0){
            printf("服务器发消息过来了");
            freopen(log_file,"a+",stdout);
            if(buff.flag==0){
                printf("\033[34m%s\033[0m: %s\n",buff.from,buff.message);
            }else if(buff.flag==2){
                printf("\033[43m 服 务 器 通 知 消 息:\033[0m: %s\n",buff.message);
            }else if(buff.flag==1){
                printf("\034[34m%s\033[0m: %s \033[5m \033[32m * \033[0m\n",buff.from,buff.message);               
            }
                fclose(stdout);
                memset(&buff,0,sizeof(Message));
            }
        
        
        close(sock_fd);
    }
}
