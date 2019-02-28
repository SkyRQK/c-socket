/*************************************************************************
	> File Name: server3.cpp
	> Author: 
	> Mail: 
	> Created Time: Thu 28 Feb 2019 09:19:03 CST
 ************************************************************************/

#include<iostream>
#include<pthread.h>
#include"common.h"
using namespace std;

struct Message{
    char from[20];
    int flag ;
    char message[1024];
};
typedef struct Node{
    int fd;
    char name[20];
    struct Node *next;
}Node,*LinkedList;

LinkedList linklist;

int peoplenum=0;



void insert( Node *node){
    peoplenum++;
    Node *p;
    p=linklist;
    while(p->next != NULL){
        p = p->next;
    }
    p->next=node;
}

void delte(LinkedList head , char * name){
    peoplenum--;
    Node *p,*q;
    p=head;
    while(p->next !=NULL && strcmp(p->next->name,name)){
        p =  p->next;
    }
    if(p->next==NULL){
        return ;
    }
    q = p->next;
    p->next=p->next->next;
    free(q);
}
void *print( void* argv ){
   LinkedList temp;
    while(1){
        temp = linklist;
        fflush(stdout);
        sleep(5);
         while(temp->next!=NULL){
            printf("%s -> %d \n",temp->next->name,temp->next->fd);
            temp = temp -> next;
        }
        printf("--------------> \n");
    }
}

void getallname(char *getname){
    LinkedList temp;
   // printf(" ss %s\n",getname);
    temp=linklist->next;
    while(temp){
     //   printf(" mei ge ren  %s\n",temp->name);
        sprintf(getname,"%s %s",getname,temp->name);
        printf("%s\n",getname);
        temp=temp->next;
    }
   // printf("%s\n",getname);
   // strncat(getname,"\n",1);
  //  printf("所有人名字 %s\n",getname);


}

int check(char *name){
    LinkedList temp;
    temp = linklist;
    while(temp->next!=NULL){
        if(strcmp(temp->next->name,name)==0){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

int checkfd(char *name){
    LinkedList temp;
    temp = linklist;
    while(temp->next!=NULL){
        if(strcmp(temp->next->name,name)==0){
            return  temp->next->fd;
        }
        temp = temp->next;
    }
    return 0;
}

void send_to (char* name,struct Message msg ){
    int fd=checkfd(name);
    send(fd,(char*)&msg,sizeof(msg),0);
}
void send_all(LinkedList linklist ,struct Message msg){
    LinkedList temp;
    temp = linklist;
    while(temp->next != NULL){
     //   printf("进来了");
        send_to(temp->next->name,msg);
        temp=temp->next;
    }
}


void *addnode( void *argv ){
    Node* p;
    p=(Node*)argv;
    insert(p);
    char welcome[10240];
    struct Message msg;
    sprintf(msg.message,"%s 已登录服务器",p->name);
    msg.flag=2;

    send_to(p->name, msg);
    send_all(linklist, msg);
    char buffer[1024] = {0};
    while(recv(p->fd,buffer,sizeof(buffer),0)>0){
        struct Message msg;
        printf("%s:%s\n",p->name,buffer);
        strcpy(msg.from,p->name);
        strcpy(msg.message,buffer);
        msg.flag=0;
        if(buffer[0] == '#'){
            char getname[300]={0};
            getallname(getname);
            sprintf(msg.message,"当前服务器人数%d 都谁啊？：%s ",peoplenum,getname);
            send_to(msg.from,msg);
            memset(getname,0,sizeof(getname));
        }else if(buffer[0]=='@'){
            char to[20] = {0};
            int i;
            for(i=0;i<20;i++){
                if(buffer[i]==' '){
                    break;
                }
            }
            strncpy(to,buffer+1,i-1);
            if(check(to)){
                msg.flag=1;
                strcpy(msg.message,buffer+i+1);
                send_to(to, msg);
            }
            else{
                msg.flag=2;
                sprintf(msg.message,"亲，您输入的是违法的用户名哦。。");
                send_to(msg.from,msg);
            }
        }else{
            send_all(linklist,msg);
        }
       // printf("%s : %s \n",p->name,buffer);
        memset(buffer,0,sizeof(buffer));
    }
    printf("用户 %s 已下线 \n",p->name);
    struct Message msg2;
    msg2.flag=2;
    strcpy(msg2.from,p->name);
    sprintf(msg2.message,"您的好友%s已下线 目前在线人数%d",p->name,peoplenum-1);
    send_all(linklist,msg2);
    delte(linklist,p->name);    
}




int main(){
    pthread_t print_t,addnode_t;
    int socketfd,server_listen,server_port;
    char temp_port[5]={0};
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    strcpy(p->name,"init");
    p->next = NULL;
    linklist = p;

    pthread_create(&print_t,NULL,print,NULL);
    
    char *config="./common.conf";
    get_conf_value(config,"Server_Port",temp_port);
   
    printf("port -> %s",temp_port);


    server_port=atoi(temp_port);


    if((server_listen = socket_create(server_port))<0){
        printf("error");
        return 1;
    }
    while(1){
        char name[20]={0};
        if((socketfd = accept(server_listen,NULL,NULL)) < 0){
             return 1;
        }
        recv(socketfd,name,sizeof(name),0);


        if(!(check(name))) {
            Node *node;
            node=(Node *)malloc(sizeof(Node));
            node->fd=socketfd;
            node->next=NULL;
            printf("用户 %s 已登录",name);
            strcpy(node->name,name);
            pthread_create(&addnode_t,NULL,addnode,(void *)node); 
        }
    }
    pthread_join(print_t,NULL);
    return 0;

}







