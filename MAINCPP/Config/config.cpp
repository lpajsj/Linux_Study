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
#include "config.h"
void config_print(void)
{
    std::cout << "/* message config*/" << std::endl;
    return;

    
}