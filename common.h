/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: 2019年02月23日 星期六 11时07分44秒
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H


#ifdef _DEBUG
#define DBG(fmt ,args...)  printf(fmt,##args)
#else
#define DBG (fmt,args...)
#endif


#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<sys/wait.h>

int get_conf_value(char *pathname,char* key_name,char* value);

int socket_create(int getport);

int socket_connect(int getport,char *host);

#endif
