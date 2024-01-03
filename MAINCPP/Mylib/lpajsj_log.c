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
    printf("test fifo %s",printf_test);
}
static uint32_t cnt1=0;
void *log_task(void *arg)
{
    printf( "/* this is log task*/");
    log_init();
    while(1){
        printf("log test %d\xd\xa",cnt1++);
        sleep(1);
    }
}