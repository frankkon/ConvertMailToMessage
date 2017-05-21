//---------------------------------------------------------------------
//邮件处理模块h文件
//---------------------------------------------------------------------
#ifndef _MAIL_H
#define _MAIL_H

#include <string>

using namespace std;

enum EContentType
{
    TEXT_PLAIN = 0,
    TEXT_HTML,
    APPLICATION_OCTETSTREAM,
    NEXT_FREE_MIME_CODE
};

enum EContentTransferEncoding
{
    _7BIT = 0,
    _8BIT,
    BINARY,
    QUOTED_PRINTABLE,
    BASE64,
    NEXT_FREE_ENCODING_CODE
};

class CMail
{
public:
    CMail();
    CMail(char* sMailInfo);
    virtual ~CMail();

    string getFrom();
    string getTo();
    string getSubject();
    EContentType getContentType();
    EContentTransferEncoding getContentTransferEncoding();
    string getContent();

    void setFrom();
    void setTo();
    void setSubject();
    void setContentType();
    void setContentTransferEncoding();
    void setContent();

    void setAllField();
    void setAllField(char* sMailInfo);
    void setMailInfo(char* sMailInfo);


private:
    string m_sFrom;
    string m_sTo;
    string m_sSubject;
    EContentType m_eContentType;
    EContentTransferEncoding m_eContentTransferEncoding;
    string m_sContent; //邮件内容
    string m_sMailInfo;//从服务端接受的所有数据。
};


#endif //_MAIL_H