//---------------------------------------------------------------------
//����������г��������ļ�
//---------------------------------------------------------------------
#ifndef _CONST_H
#define _CONST_H

//�ڴ˶�������������ʹ�õĳ���
static const int LENGTH_FILENAME  = (255+1);
static const int MAX_LENGTH_LINE = (512+1);


#define FILE_READ "r"
#define FILE_WRITE "w"
#define FILE_APPEND "a"

#define MAX_LENGTH_MESSAGE (256+1)
#define MAX_LENGTH_PASSWORD (26+1)
#define MAX_LENGTH_USERNAME (30+1)
#define MAX_LENGTH_HOSTNAME (127+1)
#define MAX_LENGTH_MAIL     1024*1024*2+1 //�ʼ�����ܳ���2M
#define MAX_NUMBER_MAIL     (500*2) //��ദ��500���ʼ�
#define MAX_LENGTH_COMMAND  (127+1)
#define MAX_LENGTH_READBUF (128+1)

#define MAX_TIMEOUT_RECV   5000  //���ճ�ʱ5��

#endif //_CONST_H
