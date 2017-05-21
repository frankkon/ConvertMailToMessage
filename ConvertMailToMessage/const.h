//---------------------------------------------------------------------
//程序处理过程中常量定义文件
//---------------------------------------------------------------------
#ifndef _CONST_H
#define _CONST_H

//在此定义程序处理过程中使用的常量
static const int LENGTH_FILENAME  = (255+1);
static const int MAX_LENGTH_LINE = (512+1);


#define FILE_READ "r"
#define FILE_WRITE "w"
#define FILE_APPEND "a"

#define MAX_LENGTH_MESSAGE (256+1)
#define MAX_LENGTH_PASSWORD (26+1)
#define MAX_LENGTH_USERNAME (30+1)
#define MAX_LENGTH_HOSTNAME (127+1)
#define MAX_LENGTH_MAIL     1024*1024*2+1 //邮件最大不能超过2M
#define MAX_NUMBER_MAIL     (500*2) //最多处理500封邮件
#define MAX_LENGTH_COMMAND  (127+1)
#define MAX_LENGTH_READBUF (128+1)

#define MAX_TIMEOUT_RECV   5000  //接收超时5秒

#endif //_CONST_H
