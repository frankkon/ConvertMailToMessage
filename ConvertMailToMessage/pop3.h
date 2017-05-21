//-------------------------------------------------------------------- 
//     Pop.h : main header file for the POP application 
//--------------------------------------------------------------------

#ifndef _POP3_H_
#define _POP3_H_

#include "const.h"
#include "socket.h"

#define CONNECTION_CHECK 0 
#define USER_CHECK  1 
#define PASSWORD_CHECK 2 
#define QUIT_CHECK  3 
#define DELETE_CHECK 4 
#define RSET_CHECK  5 
#define STAT_CHECK  6 
#define NOOP_CHECK  7 
#define LIST_CHECK  8 
#define RETR_CHECK  9

///////////////////////////////////////////////////////////////////////////// 
class CPop 
{ 
public: 
 bool List(); 
 int m_SizeOfMsg[MAX_NUMBER_MAIL];
 char* GetErrorMessage();    // If there is any error this will return it method 
 char* GetPassword();           // Getting Password stored in class 
 void SetPassword(const char* Password);    // Setting Password in class 
 char* GetUser();                   // Getting user name stored in class 
 void SetUser(const char* User);  // Setting user name in class 
 char* GetHost();                    // Getting Host name (email server name) stored in class 
 void SetHost(const char* Host);    // Setting Host name (email server name) in class 
 void SetPort(const int & Port);
 int GetPort();
 bool Connect();                    // Connecting to email server 
 int GetTotalMailSize();                // it returns the Total Mail Size 
 int GetNumberOfMails();            // It return the number of mails 
 char* GetMsgContents();
 bool Statistics();                    // issue the STAT command on email server 
 bool Retrieve(int MsgNumber);    // Getting any particular mail message 
 bool Reset();                        // issue the reset command on email server 
 int  GetMessageSize(int MsgNumber);    // Return a size of any particular mail 
 bool Noop();                     // issue the NOOP command on email server 
 bool Disconnect();            // issue the QUIT command on email server 
 bool Delete(int& MsgNumber); // Deleteing a particular message from email server 
 bool Connect(const char* Host, const char* User, const char* Password, int Port);
 void initMailBuf();
 CPop(); 
 virtual ~CPop();

private: 
    char m_ErrorMessage[MAX_LENGTH_MESSAGE];
    char m_Password[MAX_LENGTH_PASSWORD]; 
    char m_User[MAX_LENGTH_USERNAME];
    char m_Host[MAX_LENGTH_HOSTNAME];
    char* m_MsgContents;
    int m_Port;
    int m_TotalSize; 
    int m_NumberMail; 
    CSocket m_PopServer; 
    bool CheckResponse(int ResponseType);

}; 
#endif //_POP3_H_
 