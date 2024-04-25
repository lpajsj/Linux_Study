#include <iostream>
#include <getopt.h>
#include <fstream>  //文件流
#include <unistd.h>  //系统调用包括pid 延时
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <syslog.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include  <netinet/in.h>
#include <string.h>
#include "eth_task.h"
#include "lpajsj_log.h"

extern uint32_t exit_flag;
#define SERVER_PORT 8888
#define CLIENT_IP 192.168.1.3
#define CLIENT_PORT 9999
 char recvbuf[512],sendbuf[512];
void *eth_task(void *arg)
{
    struct sockaddr_in server_addr={0}; //套接字
    struct sockaddr_in client_addr={0};
    int local_ethfd,remote_ethfd;
    char ip_remote[20]={0};
    int sockaddr_lenth=sizeof(sockaddr_in); 
   
    int ret;
    uint32_t cnt=0;
    char *mode = (char *)arg;
    //文件监控
    fd_set read_fdset;
    //time
    timeval timeout;
    log_info("now eth mode is %s",mode);
    std::cout << "/* this is eth task*/" << std::endl;
    if(!strcmp(mode, "server")){
        local_ethfd=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
        server_addr.sin_family=AF_INET;
        server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        server_addr.sin_port=htons(SERVER_PORT);
        ret = bind(local_ethfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
        if(ret<0){
            perror("bind fail");
            close(local_ethfd);
            exit(EXIT_FAILURE);
        }
        ret=listen(local_ethfd,10);
        if(ret<0){
            perror("listen fail");
            close(local_ethfd);
            exit(EXIT_FAILURE);
        }
 retry:
        FD_ZERO(&read_fdset);
        FD_SET(local_ethfd,&read_fdset);
        timeout.tv_sec=5;
        timeout.tv_usec=0;
        do{
            ret=select(local_ethfd+1,&read_fdset,NULL,NULL,&timeout);
        // }while(ret<0&&errno==EINTR);
        }while(ret<0);
        remote_ethfd=accept(local_ethfd,(struct sockaddr*)&client_addr,(socklen_t*)&sockaddr_lenth);
        if(remote_ethfd<0){
            log_error("accept client error");
            exit(EXIT_FAILURE);
        }
        inet_ntop(AF_INET,(void *) &client_addr.sin_addr.s_addr,ip_remote,(socklen_t)sizeof(ip_remote));
        log_debug("client ip is %s,%d",ip_remote,client_addr.sin_port);
        printf("receive %s\n",ip_remote);
        timeout.tv_sec=1; //客户端超时时间
        timeout.tv_usec=0;
        setsockopt(remote_ethfd,SOL_SOCKET,SO_RCVTIMEO,&timeout,(socklen_t)sizeof(timeout));

    }
    else if((!strcmp(mode, "client"))){

    }
    while(1){
        // printf("eth test %d\xd\xa",cnt++);
        if(!strcmp(mode, "server")){
            memset(recvbuf,0,sizeof(recvbuf));
            ret=recv(remote_ethfd,recvbuf,sizeof(recvbuf),0);
            if(ret>0){
                log_debug("receive,%s,111",recvbuf);
                printf("receive %s\n",recvbuf);
            }
            else{
                log_error("receive error,%d",ret);
                if((ret<0&&(errno!=EAGAIN&&errno!=EWOULDBLOCK))||ret==0){
                    perror("recv error ret");
                goto retry;
                }

            }
        }
        if(exit_flag){
            // if(remote_ethfd!=0)
            close(remote_ethfd);
            close(local_ethfd);
            log_debug("eth_task exit");
            pthread_exit(NULL);
        }
        sleep(1);
    }
}

