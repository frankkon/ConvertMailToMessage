//---------------------------------------------------------------------
//邮件处理模块h文件
//---------------------------------------------------------------------
#include "mail.h"
#include "base64.h"
#include "const.h"

CMail::CMail()
{
    m_eContentType = TEXT_PLAIN;
    m_eContentTransferEncoding = _7BIT;
}

CMail::CMail(char* sMailInfo)
{
    m_sMailInfo = sMailInfo;
    m_eContentType = TEXT_PLAIN;
    m_eContentTransferEncoding = _7BIT;
}


CMail::~CMail()
{

}

string CMail::getFrom()
{
    return m_sFrom;
}

string CMail::getTo()
{
    return m_sTo;
}

string CMail::getSubject()
{
    return m_sSubject;
}

EContentType CMail::getContentType()
{
    return m_eContentType;
}

EContentTransferEncoding CMail::getContentTransferEncoding()
{
    return m_eContentTransferEncoding;
}

string CMail::getContent()
{
    return m_sContent;
}

void CMail::setFrom()
{
    string::size_type pos = m_sMailInfo.find("\nFrom:");
    if (pos != string::npos)
    {
        while (m_sMailInfo[pos] != '<')
        {
            pos++;
        }

        int iCount = 0;
        string::size_type posStart = ++pos;
        while (m_sMailInfo[pos] != '>')
        {
            pos++;
            iCount++;
        }

        m_sFrom = m_sMailInfo.substr(posStart, iCount);
    }
}

void CMail::setTo()
{
    string::size_type pos = m_sMailInfo.find("\nTo:");
    if (pos != string::npos)
    {
        while (m_sMailInfo[pos] != '<')
        {
            pos++;
        }

        int iCount = 0;
        string::size_type posStart = ++pos;
        while (m_sMailInfo[pos] != '>')
        {
            pos++;
            iCount++;
        }

        m_sTo = m_sMailInfo.substr(posStart, iCount);
    }
}

void CMail::setSubject()
{
    string::size_type pos = m_sMailInfo.find("\nSubject:");
    if (pos != string::npos)
    {
        pos = pos+10; //指向Subject首字符

        if (m_sMailInfo[pos] == '=') //等号开头，表示BASE64编码
        {
            //开头标识不确定，先将指针调整到行尾，然后反向截取
            while (m_sMailInfo[pos] != '\r' && m_sMailInfo[pos] != '\n')
            {
                pos++;
            }

            int iCount = 0;
            pos = pos - 3; //如果是BASE64，再过滤掉开头的 "？="2个字符
            while (m_sMailInfo[pos] != '?') //找到第一个？，结束
            {
                pos--;
                iCount++;
            }
            pos++;

            int iLengthDeCode = 0;
            string sEnCode = m_sMailInfo.substr(pos, iCount);
            int iEnCodeLength = sEnCode.length();   //其实3/4的长度就够了。
            char* sDeCode = new char[iEnCodeLength];
            if (sDeCode != NULL)
            {
                memset(sDeCode, 0, iEnCodeLength);
                Base64Decode(sEnCode.c_str(), sEnCode.length(), sDeCode, iLengthDeCode);
                m_sSubject = sDeCode;
                delete [] sDeCode;
                sDeCode = NULL;
            }
        }
        else
        {
            int iCount = 0;
            string::size_type posStart = pos;
            while (m_sMailInfo[pos] != '\n' && m_sMailInfo[pos] != '\r')
            {
                pos++;
                iCount++;
            }
            m_sSubject = m_sMailInfo.substr(posStart, iCount);
        }
    }
}

void CMail::setContentType()
{
    string::size_type pos = m_sMailInfo.find("\nContent-Type:");
    if (pos != string::npos)
    {
        pos = pos + 15; //指向Content-Type首字符

        int iCount = 0;
        string::size_type posStart = pos;
        while (m_sMailInfo[pos] != ';')
        {
            pos++;
            iCount++;
        }

        string sContentType = m_sMailInfo.substr(posStart, iCount);
        if (sContentType.compare("text/plain") == 0)
        {
            m_eContentType = TEXT_PLAIN;
        }
        else
        {
            m_eContentType = NEXT_FREE_MIME_CODE;
        }
    }
}

void CMail::setContentTransferEncoding()
{
    string::size_type pos = m_sMailInfo.find("\nContent-Transfer-Encoding:");
    if (pos != string::npos)
    {
        pos = pos + 28; //指向Content-Transfer-Encoding首字符

        int iCount = 0;
        string::size_type posStart = pos;
        while (m_sMailInfo[pos] != '\n' && m_sMailInfo[pos] != '\r')
        {
            pos++;
            iCount++;
        }

        string sContentTransferEncoding = m_sMailInfo.substr(posStart, iCount);
        if (sContentTransferEncoding.compare("base64") == 0)
        {
            m_eContentTransferEncoding = BASE64;
        }
        else if (sContentTransferEncoding.compare("7bit") == 0)
        {
            m_eContentTransferEncoding = _7BIT;
        }
        else
        {
            m_eContentTransferEncoding = NEXT_FREE_ENCODING_CODE;
        }
    }
}

void CMail::setContent()
{
    string::size_type pos = m_sMailInfo.find("\nX-Brightmail-Tracker: AAAAAA==");
    if (pos != string::npos)
    {
        //跳过"X-Brightmail-Tracker: AAAAAA=="行
        pos++; //跳过开头的\n
        while (m_sMailInfo[pos] != '\n')
        {
            pos++;
        }
        pos++;  //指向邮件正文行首


        int iCount = 0;
        string::size_type posStart = pos;
        while (m_sMailInfo[pos] != '.') //邮件正文以 '.'结尾。
        {
            pos++;
            iCount++;
        }

        string sEnCode = m_sMailInfo.substr(posStart, iCount);

        //如果内容较长，会存在换行，需要把换行符过滤掉。
        string::iterator it = sEnCode.begin();
        while (it != sEnCode.end())
        {
            if ( *it == '\r' || *it == '\n')
            {
                sEnCode.erase(it);
            }
            else
            {
                it++;
            }
        }


        if (m_eContentType == TEXT_PLAIN)
        {
            if (m_eContentTransferEncoding == BASE64)
            {
                int iLengthDeCode = 0;
                int iEnCodeLength = sEnCode.length();   
                char* sDeCode = new char[iEnCodeLength];    //其实3/4的长度就够了。
                if (sDeCode != NULL)
                {
                    memset(sDeCode, 0, iEnCodeLength);
                    Base64Decode(sEnCode.c_str(), sEnCode.length(), sDeCode, iLengthDeCode);
                    m_sContent = sDeCode;
                    delete[] sDeCode;
                    sDeCode = NULL;
                }
            }
            else if (m_eContentTransferEncoding == _7BIT)
            {
                m_sContent = sEnCode;
            }
            else
            {
                m_sContent = "";
            }
        }
    }
}

void CMail::setAllField()
{
    setFrom();
    setTo();
    setContentType();
    setContentTransferEncoding();
    setSubject();
    setContent();
}

void CMail::setAllField(char* sMailInfo)
{
    //暂不实现。
}

void CMail::setMailInfo(char* sMailInfo)
{
    m_sMailInfo = sMailInfo;
}



