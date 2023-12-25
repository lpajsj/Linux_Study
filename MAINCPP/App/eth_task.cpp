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
#include "eth_task.h"
uint32_t cnt=0;
void *eth_task(void *arg)
{
    std::cout << "/* this is eth task*/" << std::endl;
    while(1){
        printf("eth test %d\xd\xa",cnt++);
        sleep(1);
    }
}