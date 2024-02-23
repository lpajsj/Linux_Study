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
#include <dirent.h> //readdir
#include <libgen.h> //dirname
#include <stdbool.h>
#include <pwd.h>    //获取用户信息
#include <termio.h>  //串口驱动头文件
#include <pthread.h>
// #include "main.h"
//自定义
#include "lpajsj_log.h"
#include "fifo_buffer.h"
#define log_file_max_size 1024*1024*5   //每个log文件的最大存储量
#define log_buffer_size 4096*4          //环形buff存储大小
#define LOG_FILE_NAME "Log"             //日志文件夹
static pthread_mutex_t mutex_loglock=PTHREAD_MUTEX_INITIALIZER;  //环形buff的互斥信号量
t_fifo_buffer log_fifo_buffer;         //环形buff
uint8_t log_buffer[log_buffer_size];   //环形buff数据
uint8_t logbuf[log_buffer_size/2];     //每次写入文件的大小
// uint8_t creat_newfile=0;
uint16_t newdate=0;                    //存储新日期
uint8_t logendcnt=0;                   //同一日期末尾序号
uint8_t creatfile_flag=1;              //创建新文件
FILE *fp=NULL;                         //当前文件指针
uint32_t filesize;                     //当前文件大小
char logfilename[256];                  //文件完整路径名
/*测试数据*/
uint8_t test[]="qwertyuiopzxcvbnm,asdfghjk";
uint8_t printf_test[128];
void log_clean_file(void);
bool log_file_creat(char *file);
extern uint32_t exit_flag;
void log_init(void)
{
    uint16_t len;
    char buf[512]={0};
    char file[512]={0};
    time_t sec;
    struct tm *PTM;
    // struct timeval secus;

    fifoBuf_init(&log_fifo_buffer,log_buffer,log_buffer_size); //初始化当前buff
    // readlink("/proc/self/exe",buf,256);
    realpath("/proc/self/exe",buf);
    *strrchr(buf,'/')=0;
    // *strrchr(buf,'/')=0;
    // char  *p=dirname(buf);
    // strcpy(buf,p);
    sprintf(file,"%s/%s",buf,LOG_FILE_NAME);    //获取文件夹路径
    log_debug("%s",file);
    time(&sec);
    PTM=localtime(&sec);
    if(newdate==0)
    {
        newdate=(PTM->tm_mon+1)<<8 | (PTM->tm_mday); //获取当前月和日
    }
    //清理文件
  creatnext:
    log_clean_file(); //清理多余文件
    if(creatfile_flag>0)
    {
    if(newdate != ((PTM->tm_mon+1)<<8 | (PTM->tm_mday)))
    {
        newdate=(PTM->tm_mon+1)<<8 | (PTM->tm_mday);
        // logendcnt=0;
    }
        sprintf((char *)(logfilename),"%s/%04d%02d%02d%04d.txt",file,PTM->tm_year+1900,PTM->tm_mon+1,PTM->tm_mday,logendcnt);
        log_debug("%s",logfilename);
    }
    fp=fopen(logfilename,"r+");
    if(fp!=NULL)
    {
        fseek(fp,0,SEEK_END);
        filesize=ftell(fp);
        if(filesize>log_file_max_size)
        {
            creatfile_flag=1;
            logendcnt++;
            fclose(fp);
            log_info("open file fail %s",logfilename);
            creatfile_flag=1;
            goto creatnext;
        }
        else
        {
            log_info("open file %s",logfilename);
        }
        
    }
    else
    {
        log_file_creat(logfilename);
        filesize=0;
        creatfile_flag=0;
    }
    // fifoBuf_putData(&log_fifo_buffer,test,20);
    // len=fifoBuf_getUsed(&log_fifo_buffer);
    // fifoBuf_getData(&log_fifo_buffer,printf_test,len);
    // printf("test fifo %s\n",printf_test);
    return;
}
void log_clean_file(void)
{
    char buf[512]={0};
    char file[512]={0};
    char filenamenum[256];
    char filename[256];
    DIR *dir;
    struct dirent * dirfile;
    uint64_t curnum;
    uint64_t minfilenum=UINT64_MAX;
    uint16_t filesum=0;
    time_t sec;
    struct tm *PTM;
    uint64_t nowdatenum;
    time(&sec);
    PTM=localtime(&sec);
    nowdatenum=(((PTM->tm_year+1900)*100+PTM->tm_mon+1)*100)+PTM->tm_mday; //当前时间秒数
    realpath("/proc/self/exe",buf);
    *strrchr(buf,'/')=0;
    sprintf(file,"%s/%s",buf,LOG_FILE_NAME);//文件夹路径
    log_debug("%s",file);
    dir=opendir(file); //打开文件夹
    if(dir == NULL) //打开文件夹失败创建文件夹
    {
        openlog("MAINCPP",LOG_CONS|LOG_PID,LOG_USER); //系统日志
        syslog(LOG_ERR,"dir open error,%s",file);
        closelog();
        log_error("open fir error");
        mkdir(file,0777);
        dir=opendir(file);
    }
    while((dirfile=readdir(dir))!= NULL)//遍历所有文件
    {
        if(strcmp(dirfile->d_name,".")==0||strcmp(dirfile->d_name,"..")==0)
        {
            continue;
        }
        else if(dirfile->d_type == DT_REG)
        {
            char *name=dirfile->d_name;
            char *p=filenamenum;
            while(*name!=0)
            {
                if(*name>='0'&&*name<='9')
                {
                    memcpy(p,name,1);
                    p++;
                }
                else
                {
                    if(*name=='.'&&*(name+1)=='t'&&*(name+2)=='x'&&*(name+3)=='t')
                    {
                        filesum++;
                        curnum=strtoul(filenamenum,NULL,10);
                        log_error("%d,%d",curnum/10000,nowdatenum);
                        if(curnum/10000==nowdatenum)
                        {
                            if(newdate)
                            logendcnt++;
                            log_debug("logendcnt:%d",logendcnt);
                        }
                        if(curnum<minfilenum)
                        {
                            minfilenum=curnum;
                            memcpy(filename,dirfile->d_name,sizeof(dirfile->d_name));
                        }
                        break;
                    }
                    else
                    {
                        break;
                    }
                    
                }
                name++;
            }
        }
        else
        {
            continue;
        }
        
    }
    closedir(dir);
    if(logendcnt>9000)
    {
        dir=opendir(file);
        while((dirfile=readdir(dir))!= NULL)
        {
            if(strcmp(dirfile->d_name,".")!=0&&strcmp(dirfile->d_name,"..")!=0)
            {
            memset(file,0,sizeof(file));
            sprintf(file,"%s/%s/%s",buf,LOG_FILE_NAME,dirfile->d_name);
            if(remove(file)!=0)
            {
                log_error("remove file error");
            }
            }
        }
        closedir(dir);
    }
    if(filesum>10)
    {
        if(filename[0]!=0)
        {
            openlog("MAINCPP",LOG_CONS|LOG_PID,LOG_USER);
            syslog(LOG_DEBUG,"will remove file,%s",file);
            closelog();
            memset(file,0,sizeof(file));
            sprintf(file,"%s/%s/%s",buf,LOG_FILE_NAME,filename);
            remove(file);
            log_info("will remove file");
        }
    }
    return;

}
bool log_file_creat(char *file)
{
    log_clean_file();
    fp=fopen(file,"a+");
    if(fp==NULL)
    {
        openlog("MAINCPP",LOG_CONS|LOG_PID,LOG_USER);
        syslog(LOG_ERR,"will remove file,%s",file);
        closelog();
        log_error("creat log file error");
        return false;
    }
    return true;
}
uint32_t log_file_getwritedata(uint8_t all_fifo)
{
    uint32_t buflen;
    memset(logbuf,0,sizeof(logbuf));
    buflen = fifoBuf_getUsed(&log_fifo_buffer);
    pthread_mutex_lock(&mutex_loglock);
    if(buflen>=log_buffer_size/2)
    {
        if(buflen>=log_buffer_size)
        {
            log_warn("log fifo full");
        }
        buflen=log_buffer_size/2;
        fifoBuf_getData(&log_fifo_buffer,logbuf,buflen);
    }
    else
    {
        if(all_fifo>0&&buflen>0)
        {
            fifoBuf_getData(&log_fifo_buffer,logbuf,buflen);
        }
        else
        {
            buflen=0;
        }
        
    }
    pthread_mutex_unlock(&mutex_loglock);
    return buflen;
}
void log_file_writedata(uint8_t *buf,uint16_t len)
{
    uint32_t buflen;
    if(log_fifo_buffer.buf_ptr==NULL)
    return;
    pthread_mutex_lock(&mutex_loglock);
    fifoBuf_putData(&log_fifo_buffer,buf,len);
    pthread_mutex_unlock(&mutex_loglock);
    return;
}
void log_file_save(uint8_t all_fifo)
{
    char buf[512]={0};
    char file[512]={0};
    uint32_t buflen;
    static uint16_t timecnt=0;
    uint32_t writestatus=0;
    time_t sec;
    struct tm *PTM;
    time(&sec);
    PTM=localtime(&sec);
    realpath("/proc/self/exe",buf);
    *strrchr(buf,'/')=0;
    sprintf(file,"%s/%s",buf,LOG_FILE_NAME);
    log_debug("%s",file);
    if(log_fifo_buffer.buf_ptr==NULL)
    {
        return;
    }
saveagain:
    buflen=log_file_getwritedata(all_fifo);
    if(buflen>0)
    {
        timecnt=0;
    }
    else
    {
        timecnt++;
        if(timecnt>10)
        {
            log_file_getwritedata(0);
            timecnt=0;
        }
    }
    writestatus=fwrite(logbuf,sizeof(uint8_t),buflen,fp);
creatagain:
    if(writestatus==buflen)
    {
        if(filesize>=log_file_max_size||creatfile_flag>0)
        {
            if(newdate != ((PTM->tm_mon+1)<<8 | (PTM->tm_mday)))
            {
                newdate=(PTM->tm_mon+1)<<8 | (PTM->tm_mday);
                // logendcnt=0;
            }
            sprintf((char *)(logfilename),"%s/%04d%02d%02d%04d.txt",file,PTM->tm_year+1900,PTM->tm_mon+1,PTM->tm_mday,logendcnt);
            log_debug("%s",logfilename);
            if(fp!=NULL)
            {
                fclose(fp);
            }
            fp=fopen(logfilename,"r+");
            if(fp!=NULL)
            {

                fseek(fp,0,SEEK_END);
                filesize=ftell(fp);
                if(creatfile_flag>0)
                {
                    if(filesize>log_file_max_size)
                    {
                        creatfile_flag=1;
                        logendcnt++;
                        fclose(fp);
                        log_info("open file fail %s",logfilename);
                        creatfile_flag=1;
                        goto creatagain;
                    }
                    else
                    {
                        log_info("open file %s",logfilename);
                        creatfile_flag=0;
                    }
                }
            }
            else
            {
                log_file_creat(logfilename);
                filesize=0;
                creatfile_flag=0;
            }
        }
    }
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
    // len=strlen(p);
    // sprintf((char *)&p[len],"\xd\xa");
    strcat(p,"\xd\xa");
    len=strlen(p);
    log_file_writedata(p,len);
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
    // len=strlen(p);
    // sprintf((char *)&p[len],"\xd\xa");
    strcat(p,"\xd\xa");
    len=strlen(p);
    log_file_writedata(p,len);
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
    // len=strlen(p);
    // sprintf((char *)&p[len],"\xd\xa");
    strcat(p,"\xd\xa");
    len=strlen(p);
    log_file_writedata(p,len);
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
    // len=strlen(p);
    // sprintf((char *)&p[len],"\xd\xa");
    strcat(p,"\xd\xa");
    len=strlen(p);
    log_file_writedata(p,len);
    return;
}
// static uint32_t cnt1=0;
void *log_task(void *arg)
{
    log_info( "/* this is info test*/");
    log_warn( "/* this is warn test*/");
    log_debug( "/* this is debug test*/");
    log_error( "/* this is error test*/");
    log_init();
    while(1){
        // printf("log test %d\xd\xa",cnt1++);
        log_file_save(0);
        sleep(1);
        if(exit_flag){
            log_file_save(1);
            pthread_exit(NULL);
        }
    }

}