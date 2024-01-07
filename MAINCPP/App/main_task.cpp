#include <iostream>
#include <getopt.h>
#include <fstream>  //文件流
#include <unistd.h>  //系统调用包括pid 延时
#include<sys/stat.h>
#include<sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <syslog.h>
#include "main_task.h"
void main_task_print(void)
{
    std::cout << "/* message main_task*/" << std::endl;
    return;
}
static uint32_t cnt=0;
void * main_task(void *arg)
{
    std::cout << "/* this is main task*/" << std::endl;
    while(1){
        // printf("main test %d\xd\xa",cnt++);
        sleep(1);
    }
}