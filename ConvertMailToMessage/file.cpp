//---------------------------------------------------------------------
//文本文件处理类
//---------------------------------------------------------------------
#include "file.h"
#include "return_code.h"
#include "const.h"
#include <cstring>

using namespace std;

CFile::CFile():m_pFile(NULL)
{
    memset(m_sFilename, 0, LENGTH_FILENAME);
}

CFile::CFile(const char* filename):m_pFile(NULL)
{
    strncpy_s(m_sFilename, LENGTH_FILENAME, filename, LENGTH_FILENAME);
}

CFile::~CFile()
{
    if (m_pFile)
    {
        fclose(m_pFile);
    }
    memset(m_sFilename, 0, LENGTH_FILENAME);
}    

const char* CFile::getFileName()
{
    return m_sFilename;
}

int CFile::setFileName(const char* const sFileName)
{
    if (NULL == sFileName)
    {
        return RETURN_CODE_ERROR;
    }
    strncpy_s(m_sFilename, LENGTH_FILENAME, sFileName, LENGTH_FILENAME);
    return RETURN_CODE_OK;
}

int CFile::openForRead()
{
    if (!strlen(m_sFilename))
    {
        return RETURN_CODE_ERROR;
    }

    int iErrNo = fopen_s(&m_pFile, m_sFilename, FILE_READ);
    if (0 == iErrNo)
    {
        return RETURN_CODE_OK;
    }
    else
    {
        return RETURN_CODE_ERROR;
    }
}

int CFile::openForWrite()
{
    if (!strlen(m_sFilename))
    {
        return RETURN_CODE_ERROR;
    }

    int iErrNo = fopen_s(&m_pFile, m_sFilename, FILE_WRITE);
    if (0 == iErrNo)
    {
        return RETURN_CODE_OK;
    }
    else
    {
        return RETURN_CODE_ERROR;
    }
}

int CFile::openForAppend()
{
    if (!strlen(m_sFilename))
    {
        return RETURN_CODE_ERROR;
    }

    int iErrNo = fopen_s(&m_pFile, m_sFilename, FILE_APPEND);
    if (0 == iErrNo)
    {
        return RETURN_CODE_OK;
    }
    else
    {
        return RETURN_CODE_ERROR;
    }
}

int CFile::close()
{
    if (NULL != m_pFile)
    {
        fclose(m_pFile);
    }
    m_pFile = NULL;
    return RETURN_CODE_OK;
}

int CFile::write(char* pBuf, int iLength)
{
    if (NULL == m_pFile || 0 == strlen(m_sFilename))
    {
        return RETURN_CODE_ERROR;
    }

    if (NULL == pBuf)
    {
        return RETURN_CODE_ERROR;
    }

    if (fwrite(pBuf, sizeof(char), iLength, m_pFile))
    {
        return RETURN_CODE_OK;
    }
    else
    {
        return RETURN_CODE_ERROR;
    }
}

int CFile::read(char* pBuf, int iLength)
{
    if (NULL == m_pFile || 0 == strlen(m_sFilename))
    {
        return RETURN_CODE_ERROR;
    }

    if (NULL == pBuf)
    {
        return RETURN_CODE_ERROR;
    }

    if (fread(pBuf, sizeof(char), iLength, m_pFile))
    {
        return RETURN_CODE_OK;
    }
    else
    {
        return RETURN_CODE_ERROR;
    }
}

int CFile::append(char* pBuf, int iLength)
{
    if (NULL == m_pFile || 0 == strlen(m_sFilename))
    {
        return RETURN_CODE_ERROR;
    }

    if (NULL == pBuf)
    {
        return RETURN_CODE_ERROR;
    }

    if (fwrite(pBuf, sizeof(char), iLength, m_pFile))
    {
        return RETURN_CODE_OK;
    }
    else
    {
        return RETURN_CODE_ERROR;
    }
}

int CFile::readALine(char* sLine, int iLength)
{
    if ( NULL == sLine)
    {
        return RETURN_CODE_ERROR;
    }
    
    if (NULL == fgets(sLine, iLength, m_pFile))
    {
        return RETURN_CODE_ERROR;
    }
    else
    {
        return RETURN_CODE_OK;
    }
}


