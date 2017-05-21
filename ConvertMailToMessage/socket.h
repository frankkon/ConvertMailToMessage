//--------------------------------------------------------------
// Socket¿‡∂®“Â
//--------------------------------------------------------------
#ifndef _SOCKET_H
#define _SOCKET_H

#include <winsock2.h>

class CSocket
{
private:
    SOCKET m_socket;
public:
    CSocket();
    virtual ~CSocket();

    void create();
    void close();
    int connectHost(const char* pHost, unsigned short iPort);
    void sendToHost(const char* sCmd, int iCmdLength);
    void receiveFromHost(char* pBuf, int iBufLength);
};


#endif //_SOCKET_H
