//---------------------------------------------------------------------
//单元测试cpp文件
//---------------------------------------------------------------------
#include "unit_test.h"
#include "return_code.h"
#include <iostream>
#include <fstream>

#include "file.h"
#include "config_file.h"
#include "pop3.h"
#include "mail.h"
#include "base64.h"

using namespace std;

int unitTest()
{   
    cout << "------------单元测试开始------------" << endl;

/*    char decode[200] = {0};
    int l = 0;
    string encode = "d28gYWkgYmVpamluZyB0aWFuYW5tZW7O0rCusbG+qczssLLDxaGjIA0KDQoyMDE0LTA3LTA0IA0KDQoNCg0Ka29uX2p1biANCg==";
    Base64Decode(encode.c_str(), encode.length(), decode, l);
    cout << decode << endl;
*/
    cout << "------------CFile类测试开始------------" << endl;
    CFile myFile;
    myFile.setFileName("abc.txt");
    cout << "setFileName = abc.txt, getFileName = " << myFile.getFileName() << endl;

    CFile myFile2("abc.txt");
    myFile2.openForWrite();
    myFile2.write("I am frankkon.\n", 15);
    myFile2.close();

    myFile2.openForAppend();
    myFile2.append("what is your name?", 18);
    myFile2.close();

    myFile2.openForRead();
    char buf[256] = { 0 };
    cout << "file content is :" << endl;
    myFile2.read(buf, 255);
    cout << buf << endl;  
    cout << "------------CFile类测试开始------------" << endl;

    cout << "------------CConfigFile类测试开始------------" << endl;
    CConfigFile myConfigFile("config.ini");
    myConfigFile.openForRead();
    char sDBName[128] = {0};
    myConfigFile.getFieldValue("DBName", sDBName);
    cout << "DBName = " << sDBName << endl;
    myConfigFile.close();
    cout << "------------CConfigFile类测试开始------------" << endl;


    cout << "------------CPop类测试开始------------" << endl;
    CConfigFile mymailConfigFile("mailconfig.ini");
    mymailConfigFile.openForRead();
    char sHostName[128] = { 0 };
    char sPort[10] = {0};
    int iPort = 0;
    char sUserName[20] = {0};
    char sPassword[20] = {0};
    mymailConfigFile.getFieldValue("HostName", sHostName);
    mymailConfigFile.getFieldValue("Port", sPort);
    iPort = atoi(sPort);
    cout << "HostName = " << sHostName << endl;
    cout << "Port = " << iPort << endl;

    mymailConfigFile.getFieldValue("UserName", sUserName);
    mymailConfigFile.getFieldValue("Password", sPassword);
    cout << "UserName = " << sUserName << endl;
    cout << "Password = " << sPassword << endl;
    mymailConfigFile.close();
    CPop pop;
    if (pop.Connect(sHostName, sUserName, sPassword, iPort))
    {
        if (pop.Statistics())
        {
            cout << "STAT Command :" << pop.GetNumberOfMails() << endl;
        }
        else
        {
            cout << "STAT error! Error Message is :" << pop.GetErrorMessage() << endl;
        }

        if (pop.List())
        {
            cout << "LIST Command :" << endl;
            int iMailCount = pop.GetNumberOfMails();
            for (int i = 0; i < iMailCount*2; i = i + 2)
            {
                cout <<"第" << pop.m_SizeOfMsg[i] << "封邮件，大小为" << pop.m_SizeOfMsg[i+1] << "字节" << endl;
            }
        }
        else
        {
            cout << "LIST error! Error Message is :" << pop.GetErrorMessage() << endl;
        }

        cout << "RETR Command :" << endl;
        int iMailCount = pop.GetNumberOfMails();
        ofstream of;
        char filename[20] = {0};
        for (int i = 1; i <= iMailCount; i++)
        {
            pop.Retrieve(i);
            sprintf_s(filename, "mail%d.txt", i);
            of.open(filename);
            of << pop.GetMsgContents() << endl;
            of.close();
            cout << "第" << i << "封邮件，已经成功写入" << filename<< endl;

            CMail mail(pop.GetMsgContents());
            mail.setAllField();
            cout << "From:" << mail.getFrom() << endl;
            cout << "To:" << mail.getTo() << endl;
            cout << "Subject:" << mail.getSubject() << endl;
            cout << "ContentType:" << mail.getContentType() << endl;
            cout << "ContentTransferEncoding:" << mail.getContentTransferEncoding() << endl;
            cout << "Content:" << endl  << mail.getContent() << endl;

        }

        pop.Disconnect();
    }
    else
    {
        cout << "connect error! Error Message is :" << pop.GetErrorMessage() << endl;
    }
    cout << "------------CPop类测试结束------------" << endl;

    cout << "------------单元测试结束------------" << endl;

    return RETURN_CODE_OK;
}


