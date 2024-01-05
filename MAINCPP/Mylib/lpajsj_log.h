#ifndef LPAJSJ_LOG_H_
#define LPAJSJ_LOG_H_

#ifdef __cplusplus
 extern "C" {
#endif
void log_init(void);
void *log_task(void *arg);



void _log_info(char *FILE,uint16_t line,const char *format, ...);
void _log_warn(char *FILE,uint16_t line,const char *format, ...);
void _log_debug(char *FILE,uint16_t line,const char *format, ...);
void _log_error(char *FILE,uint16_t line,const char *format, ...);
#define log_info(p,...) _log_info(__FILE__,__LINE__,p,##__VA_ARGS__)
#define log_warn(p,...) _log_warn(__FILE__,__LINE__,p,##__VA_ARGS__)
#define log_debug(p,...) _log_debug(__FILE__,__LINE__,p,##__VA_ARGS__)
#define log_error(p,...) _log_error(__FILE__,__LINE__,p,##__VA_ARGS__)

#ifdef __cplusplus
 }
#endif
#endif