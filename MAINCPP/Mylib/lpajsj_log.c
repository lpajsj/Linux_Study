// #include <iostream>
#include <getopt.h>
// #include <fstream>  //文件流
#include <unistd.h>  //系统调用包括pid 延时
#include<sys/stat.h>
#include<sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <syslog.h>
#include <signal.h>
#include <stdarg.h> //va_list
#include <sys/time.h> //gettimeofday
#include <string.h>

#include <pwd.h>    //获取用户信息
#include <termio.h>  //串口驱动头文件

#include <pthread.h>
// #include "main.h"

//自定义

#include "lpajsj_log.h"
#include "fifo_buffer.h"

#define log_buffer_size 1024

t_fifo_buffer log_fifo_buffer;

uint8_t log_buffer[log_buffer_size];
uint8_t test[]="qwertyuiopzxcvbnm,asdfghjk";
uint8_t printf_test[128];


void log_init(void)
{
    uint16_t len;
    fifoBuf_init(&log_fifo_buffer,log_buffer,log_buffer_size);
    fifoBuf_putData(&log_fifo_buffer,test,20);
    len=fifoBuf_getUsed(&log_fifo_buffer);
    fifoBuf_getData(&log_fifo_buffer,printf_test,len);
    printf("test fifo %s\n",printf_test);
}
void _log_info(char *file,uint16_t line,const char *format, ...)
{
    va_list args;
    char p[256]={0};
    time_t sec;
    struct tm *PTM;
    struct timeval secus;
    file=strrchr(file,'/')+1;
    time(&sec);
    PTM=localtime(&sec);
    gettimeofday(&secus,NULL);
    uint64_t timesecms=secus.tv_sec*1000+secus.tv_usec/1000;  
    uint16_t len=sprintf(p,"[%04d-%02d-%02d] [%08lX] file:%s line:%d debug:",PTM->tm_year+1900,PTM->tm_mon+1,PTM->tm_mday,timesecms,file,line);
    va_start(args,format);
    // p=va_arg(args,char *);
    vsnprintf(p+len,256-len,format,args);
    va_end(args);
    printf("\033[0m %s\033[0m\xd\xa",p);
    return;
}
void _log_warn(char *file,uint16_t line,const char *format, ...)
{
    va_list args;
    char p[256]={0};
    time_t sec;
    struct tm *PTM;
    struct timeval secus;
    file=strrchr(file,'/')+1;
    time(&sec);
    PTM=localtime(&sec);
    gettimeofday(&secus,NULL);
    uint64_t timesecms=secus.tv_sec*1000+secus.tv_usec/1000;  
    uint16_t len=sprintf(p,"[%04d-%02d-%02d] [%08lX] file:%s line:%d debug:",PTM->tm_year+1900,PTM->tm_mon+1,PTM->tm_mday,timesecms,file,line);
    va_start(args,format);
    // p=va_arg(args,char *);
    vsnprintf(p+len,256-len,format,args);
    va_end(args);
    printf("\033[33m %s\033[0m\xd\xa",p);
    return;
}
void _log_debug(char *file,uint16_t line,const char *format, ...)
{
    va_list args;
    char p[256]={0};
    time_t sec;
    struct tm *PTM;
    struct timeval secus;
    file=strrchr(file,'/')+1;
    time(&sec);
    PTM=localtime(&sec);
    gettimeofday(&secus,NULL);
    uint64_t timesecms=secus.tv_sec*1000+secus.tv_usec/1000;  
    uint16_t len=sprintf(p,"[%04d-%02d-%02d] [%08lX] file:%s line:%d debug:",PTM->tm_year+1900,PTM->tm_mon+1,PTM->tm_mday,timesecms,file,line);
    va_start(args,format);
    // p=va_arg(args,char *);
    vsnprintf(p+len,256-len,format,args);
    va_end(args);
    printf("\033[32m %s\033[0m\xd\xa",p);
    return;
}
void _log_error(char *file,uint16_t line,const char *format, ...)
{
    va_list args;
    char p[256]={0};
    time_t sec;
    struct tm *PTM;
    struct timeval secus;
    file=strrchr(file,'/')+1;
    time(&sec);
    PTM=localtime(&sec);
    gettimeofday(&secus,NULL);
    uint64_t timesecms=secus.tv_sec*1000+secus.tv_usec/1000;  
    uint16_t len=sprintf(p,"[%04d-%02d-%02d] [%08lX] file:%s line:%d debug:",PTM->tm_year+1900,PTM->tm_mon+1,PTM->tm_mday,timesecms,file,line);
    va_start(args,format);
    // p=va_arg(args,char *);
    vsnprintf(p+len,256-len,format,args);
    va_end(args);
    printf("\033[31m %s\033[0m\xd\xa",p);
    return;
}
static uint32_t cnt1=0;
void *log_task(void *arg)
{
    log_info( "/* this is info test*/");
    log_warn( "/* this is warn test*/");
    log_debug( "/* this is debug test*/");
    log_error( "/* this is error test*/");
    log_init();
    while(1){
        printf("log test %d\xd\xa",cnt1++);
        sleep(1);
    }
}