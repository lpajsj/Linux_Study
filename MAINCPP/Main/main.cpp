
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
#include <signal.h>

#include <pwd.h>    //获取用户信息
#include <termio.h>  //串口驱动头文件

#include <pthread.h>
// #include "main.h"
//自定义
#include "config.h"
#include "main_task.h"
#include "eth_task.h"
#include "lpajsj_log.h"

char BuildTime[]={"BuildTime[" __TIME__ " " __DATE__ "]"};
pthread_t main_thread;
pthread_t eth_thread;
pthread_t log_thread;
static const struct option long_options[] = {
			{"test", no_argument, NULL,'t'},
			{"help",	no_argument,NULL, 'h'},
			{"goal",	no_argument,NULL, 'g'},
			{0,		0,					0,  0}
};
static void usage(FILE *fp, int argc, char **argv)
{
	fprintf(fp,
			 "usage: %s [options]\n\n"
			 "Version %s \n"
			 "Options:\n"
			 "-t | --test	       my test code\n"
			 "-h | --help		   Print this message\n"
			 "-g | --goal		   The goal of this code\n"
			 "",
			 argv[0], BuildTime);
}
static const char short_options[] = "thga";
void signal_handler(int sig)
{
	printf("\033[31m system stop\033[0m\xd\xa");
	printf("\033[32m system stop\033[0m\xd\xa");
	printf("\033[33m system stop\033[0m\xd\xa");
	signal(sig,SIG_DFL);  //恢复默认处理方法
}
int main(int argc, char* argv[])
{
	static uint32_t cnt=0;
	int ret=1;
	if(argc >0){
	for(;;){
		int opt_idx;
		int opt_nxt;
		opt_nxt = getopt_long(argc, argv, short_options, long_options, &opt_idx);
		if(opt_nxt < 0){
		std::cout << "/*Please enter the correct parameters*/" <<opt_idx<<"   "<<opt_nxt<< std::endl;
		std::cout << "/*for example ./hello -h */" << std::endl;
			break;
		}

		switch(opt_nxt){
			case '0':
				std::cout << "/* input 0 */" << std::endl;
				exit(EXIT_SUCCESS);
				break;
			case 't':
				std::cout << "/* input t */" << std::endl;
				config_print();
				main_task_print();
				break;
			case 'h':
				std::cout << "/* input h */" << std::endl;
				usage(stdout, argc, argv);
				exit(EXIT_SUCCESS);
				break;
			case 'm':
				std::cout << "/*The goal of this code is to motivate me to learn*/" << std::endl;
				exit(EXIT_SUCCESS);
			break;
			case 'a':
				std::cout << "/* input a */" << std::endl;
				exit(EXIT_SUCCESS);
				break;
			default:
				usage(stderr, argc, argv);
				exit(EXIT_FAILURE);
				break;
			}
		}
		signal(SIGINT,signal_handler);//ctrl C
		printf("\033[31m system start\033[0m\xd\xa");
		printf("\033[32m system start\033[0m\xd\xa");
		printf("\033[33m system start\033[0m\xd\xa");
		std::cout << "/* input a */" << std::endl;
		ret=pthread_create(&main_thread,NULL,main_task,NULL);
		ret=pthread_create(&eth_thread,NULL,eth_task,NULL);
		ret=pthread_create(&log_thread,NULL,log_task,NULL);

		while(1)
		{
			printf("test %d\xd\xa",cnt++);
			sleep(1);
		}
		exit(EXIT_SUCCESS);
	}
}