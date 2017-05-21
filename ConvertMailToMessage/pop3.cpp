//--------------------------------------------------------------
// POP3协议处理类实现
//--------------------------------------------------------------
#include <cstring>
#include <cstdlib>
#include <cstdio>

//#include <WinBase.h>
#include "pop3.h"

CPop::CPop() :m_Port(110), m_TotalSize(0), m_NumberMail(0)
{ 
    memset(m_SizeOfMsg, 0, sizeof(m_SizeOfMsg));
    memset(m_ErrorMessage, 0, sizeof(m_ErrorMessage));
    memset(m_Password, 0, sizeof(m_Password));
    memset(m_User, 0, sizeof(m_User));
    memset(m_Host, 0, sizeof(m_Host));
    m_MsgContents = NULL;
    m_PopServer.create();

} 

CPop::~CPop() 
{ 
     m_PopServer.close(); 
     if (NULL != m_MsgContents)
     {
         delete [] m_MsgContents;
     }
}

bool CPop::Connect(const char* Host, const char* User, const char* Password, int Port = 110 )
{ 
    char sCmd[MAX_LENGTH_COMMAND] = {0};

    if (!m_PopServer.connectHost(Host, Port))
    { 
        strncpy_s(m_ErrorMessage, MAX_LENGTH_MESSAGE, "Server cannot be connected", 26);
        return false; 
    } 
    else 
    { 
        if(CheckResponse(CONNECTION_CHECK)==false) return false;

        sprintf_s(sCmd, MAX_LENGTH_COMMAND, "USER %s\r\n", User);
        m_PopServer.sendToHost(sCmd, strlen(sCmd));
        if(CheckResponse(USER_CHECK)==false) return false;

        sprintf_s(sCmd, MAX_LENGTH_COMMAND, "PASS %s\r\n", Password);
        m_PopServer.sendToHost(sCmd, strlen(sCmd));
        if (CheckResponse(PASSWORD_CHECK)==false) return false;

        return true; 
    }

}

bool CPop::Delete(int& MsgNumber) 
{ 
    char sCmd[MAX_LENGTH_COMMAND];

    sprintf_s(sCmd, MAX_LENGTH_COMMAND, "DELE %d\r\n", MsgNumber);
    m_PopServer.sendToHost(sCmd, strlen(sCmd));
    return CheckResponse(DELETE_CHECK);
}

bool CPop::Disconnect() 
{ 
    char sCmd[MAX_LENGTH_COMMAND];

    sprintf_s(sCmd, MAX_LENGTH_COMMAND, "QUIT\r\n");
    m_PopServer.sendToHost(sCmd, strlen(sCmd));

    return CheckResponse(QUIT_CHECK); 
}

bool CPop::Noop() 
{ 
    char sCmd[MAX_LENGTH_COMMAND];

    sprintf_s(sCmd, MAX_LENGTH_COMMAND, "NOOP\r\n");
    m_PopServer.sendToHost(sCmd, strlen(sCmd));

    return CheckResponse(NOOP_CHECK); 

}

// Return the Msg Size for given msg number 
int CPop::GetMessageSize(int MsgNumber) 
{ 
    if (0 < MsgNumber && MsgNumber <= MAX_NUMBER_MAIL/2)
    {
        return m_SizeOfMsg[MsgNumber*2-1];
    }
    else
    {
        return 0;
    }

}

bool CPop::Reset() 
{ 
    char sCmd[MAX_LENGTH_COMMAND];

    sprintf_s(sCmd, MAX_LENGTH_COMMAND, "RSET\r\n");
    m_PopServer.sendToHost(sCmd, strlen(sCmd));

    return CheckResponse(RSET_CHECK); 
}

// MsgContents will hold the msg body 
bool CPop::Retrieve(int  MsgNumber) 
{ 
    char sCmd[MAX_LENGTH_COMMAND];

    sprintf_s(sCmd, MAX_LENGTH_COMMAND, "RETR %d\r\n", MsgNumber);
    m_PopServer.sendToHost(sCmd, strlen(sCmd));
    initMailBuf();
    return CheckResponse(RETR_CHECK); 
}

bool CPop::Statistics() 
{ 
    char sCmd[MAX_LENGTH_COMMAND];

    sprintf_s(sCmd, MAX_LENGTH_COMMAND, "STAT\r\n");
    m_PopServer.sendToHost(sCmd, strlen(sCmd));

    return CheckResponse(STAT_CHECK); 
}

char* CPop::GetMsgContents() 
{ 
 return m_MsgContents; 
}

int CPop::GetNumberOfMails() 
{ 
 return m_NumberMail; 
}

int CPop::GetTotalMailSize() 
{ 
 return m_TotalSize; 
}

bool CPop::Connect() 
{ 
 Connect(m_Host, m_User, m_Password, m_Port); 
 return true; 
}

void CPop::SetHost(const char* Host) 
{ 
    strncpy_s(m_Host, MAX_LENGTH_HOSTNAME, Host, strlen(Host));
}

char* CPop::GetHost() 
{ 
 return m_Host; 
}

void CPop::SetUser(const char* User) 
{ 
    strncpy_s(m_User, MAX_LENGTH_USERNAME, User, strlen(User));
}

char* CPop::GetUser() 
{ 
 return m_User; 
}

void CPop::SetPassword(const char* Password) 
{ 
    strncpy_s(m_Password, MAX_LENGTH_PASSWORD, Password, strlen(Password));

}

char* CPop::GetPassword()
{ 
 return m_Password; 
}

void CPop::SetPort(const int & Port)
{
    m_Port = Port;
}

int CPop::GetPort()
{
    return m_Port;
}

bool CPop::CheckResponse(int ResponseType) 
{ 
    char buf[MAX_LENGTH_MESSAGE] = { 0 };

    //因为邮件比较大，单独处理，否则统一在此接收
    if (ResponseType != RETR_CHECK)
    {
        m_PopServer.receiveFromHost(buf, sizeof(buf));
    }

    switch (ResponseType) 
    { 
        case CONNECTION_CHECK: 
            if (_strnicmp(buf,"-ERR", 4) == 0) 
            { 
                strncpy_s(m_ErrorMessage, MAX_LENGTH_MESSAGE, "Bad Connection.", 15);
                return false;
            } 
        break;
        case USER_CHECK: 
            if (_strnicmp(buf,"-ERR", 4) == 0) 
            { 
                strncpy_s(m_ErrorMessage, MAX_LENGTH_MESSAGE, "Bad User Name.", 14);
                return false;
            } 
        break; 
        case PASSWORD_CHECK: 
            if (_strnicmp(buf,"-ERR", 4) == 0) 
            { 
                strncpy_s(m_ErrorMessage, MAX_LENGTH_MESSAGE, "Bad Password.", 13);
                return false;
            } 
        break; 
        case QUIT_CHECK: 
            if (_strnicmp(buf,"-ERR", 4) == 0) 
            { 
                strncpy_s(m_ErrorMessage, MAX_LENGTH_MESSAGE, "Error occured during QUIT.", 26);
                return false;
            } 
        break; 
        case DELETE_CHECK: 
            if (_strnicmp(buf,"-ERR", 4) == 0) 
            { 
                strncpy_s(m_ErrorMessage, MAX_LENGTH_MESSAGE, "Error occured during DELE.", 26);
                return false;
            } 
        break; 
        case RSET_CHECK: 
            if (_strnicmp(buf,"-ERR", 4) == 0) 
            { 
                strncpy_s(m_ErrorMessage, MAX_LENGTH_MESSAGE, "Error occured during RSET.", 26);
                return false;
            } 
        break; 
        case STAT_CHECK: 
            if (_strnicmp(buf,"-ERR", 4) == 0) 
            { 
                strncpy_s(m_ErrorMessage, MAX_LENGTH_MESSAGE, "Error occured during STAT.", 26);
                return false;
            } 
            else 
            { 
                //STAT命令成功返回格式为：+OK NumberOfEmail TotalSizeOfEmail,
                //例如：+OK 2 26895，表示2封邮件，大小为26895
                bool EmailNumber = true; 
                for (char *p = buf; *p != '\0'; p++) 
                { 
                    if (*p == '\t' || *p == ' ') 
                    { 
                        if(EmailNumber == true) 
                        { 
                            m_NumberMail = atoi(p); 
                            EmailNumber = false; 
                        } 
                        else 
                        { 
                            m_TotalSize = atoi(p); 
                            return true; 
                        } 
                    } 
                }
            } 
        break; 
        case NOOP_CHECK: 
            if (_strnicmp(buf,"-ERR", 4) == 0) 
            { 
                strncpy_s(m_ErrorMessage, MAX_LENGTH_MESSAGE, "Error occured during NOOP.", 26);
                return false;
            } 
        break;
        case LIST_CHECK: 
            if (_strnicmp(buf,"-ERR", 4) == 0) 
            { 
                strncpy_s(m_ErrorMessage, MAX_LENGTH_MESSAGE, "Error occured during LIST.", 26);
                return false;
            } 
            else 
            { 
                //LIST命令成功返回格式为：+OK {NumberOfEmail SizeOfEmail}.
                //例如：+OK 1 115 2 334 3 556. 以“.”结束
                char* p = NULL;
                int index = 0;

                //先过滤掉第一行+OK行
                p = strstr(buf, "\n");
                for (; *p != '.'; p++)
                {
                    if (*p == '\n' || *p == ' ')
                    {
                        m_SizeOfMsg[index] = atoi(p+1);
                        index++;
                    }
                }
            } 
        break; 
        case RETR_CHECK: 
            m_PopServer.receiveFromHost(m_MsgContents, MAX_LENGTH_MAIL);
            if (_strnicmp(m_MsgContents, "-ERR", 4) == 0)
            { 
                strncpy_s(m_ErrorMessage, MAX_LENGTH_MESSAGE, "Error occured during RETR.", 26);
                return false;
            } 
        break; 
    } 
    return true; 
}

char* CPop::GetErrorMessage() 
{ 
     return m_ErrorMessage; 
}

bool CPop::List() 
{ 
    char sCmd[MAX_LENGTH_COMMAND];

    sprintf_s(sCmd, MAX_LENGTH_COMMAND, "LIST\r\n");
    m_PopServer.sendToHost(sCmd, strlen(sCmd));
    return CheckResponse(LIST_CHECK); 
} 
void CPop::initMailBuf()
{
    if (NULL != m_MsgContents)
    {
        delete m_MsgContents;
    }

    m_MsgContents = new char[MAX_LENGTH_MAIL];  
    memset(m_MsgContents, 0, MAX_LENGTH_MAIL);
}