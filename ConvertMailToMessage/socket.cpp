//--------------------------------------------------------------
// Socket类实现
//--------------------------------------------------------------
#include "socket.h"
#include <iostream>
#include <io.h>

#include "const.h"
#include "return_code.h"

using namespace std;

CSocket::CSocket():m_socket(0)
{
    
}

CSocket::~CSocket()
{
    if (0 != m_socket)
    {
        closesocket(m_socket);
    }
}

void CSocket::create()
{
    //windows必须的初始步骤----begin-----。
#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    int iErr = 0;
    wVersionRequested = MAKEWORD(2, 2);
    iErr = WSAStartup(wVersionRequested, &wsaData);
    if (0 != iErr)
    {
        cout << "WSAStartup 建立失败" << endl;
        cout << "错误码=" << iErr << endl;
    }
#endif
    //windows必须的初始步骤----end  -----。

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == m_socket)
    {
        cout << "socket 建立失败" << endl;
        cout << "错误码=" << WSAGetLastError() << endl;
    }
}

void CSocket::close()
{
    closesocket(m_socket);
    //windows必须的清理步骤----begin-----。
#ifdef WIN32
    WSACleanup();
#endif
    //windows必须的清理步骤----end  -----。
}

int CSocket::connectHost(const char* pHost, unsigned short iPort)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(iPort);
    struct in_addr inAddr;
    inAddr.s_addr = inet_addr(pHost);
    addr.sin_addr = inAddr;
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

    int iErr = 0;
    iErr = connect(m_socket, (struct sockaddr *)(&addr), sizeof(addr));
    if (SOCKET_ERROR == iErr)
    {
        cout << "connect 建立失败" << endl;
        cout << "错误码=" << WSAGetLastError() << endl;
        return RETURN_CODE_CONNECT_ERROR;
    }

    unsigned long timeout = MAX_TIMEOUT_RECV;
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    return RETURN_CODE_CONNECT_OK;
}

void CSocket::sendToHost(const char* sCmd, int iCmdLength)
{
    int iErr = 0;
    iErr = send(m_socket, sCmd, iCmdLength, 0);
    if (SOCKET_ERROR == iErr)
    {
        cout << "send 建立失败" << endl;
        cout << "错误码=" << WSAGetLastError() << endl;
    }
}

void CSocket::receiveFromHost(char* pBuf, int iBufLength)
{
    int iLen = 0;
    int iPos = 0;
    char sbuf[MAX_LENGTH_READBUF] = { 0 };

    while ((iLen = recv(m_socket, sbuf, MAX_LENGTH_READBUF-1, 0)) > 0)
    {
        strncpy_s(pBuf + iPos, iBufLength, sbuf, iLen);
        iPos = iPos + iLen;
        iBufLength = iBufLength - iLen;
    }



}

