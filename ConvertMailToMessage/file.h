//---------------------------------------------------------------------
//文本文件处理类
//---------------------------------------------------------------------
#ifndef _FILE_H
#define _FILE_H

#include <cstdio>
#include "const.h"

using namespace std;

class CFile
{
private:
    FILE* m_pFile;
    char m_sFilename[LENGTH_FILENAME];
public:
    CFile();
    CFile(const char* filename);
    virtual ~CFile();

    const char* getFileName();
    int setFileName(const char* const sFileName);

    int openForRead();
    int openForWrite();
    int openForAppend();
    int close();
    int write(char* pBuf, int iLength);
    int read(char* pBuf, int iLength);
    int append(char* pBuf, int iLength);
    int readALine(char* sLine, int iLength);
};


#endif //_FILE_H
