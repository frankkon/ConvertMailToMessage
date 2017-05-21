//---------------------------------------------------------------------
//�����ʼ���ļ�������
//---------------------------------------------------------------------

#include <cstring>
#include <cstdlib>

#include "config_file.h"
#include "const.h"
#include "return_code.h"

CConfigFile::CConfigFile()
{

}

CConfigFile::CConfigFile(const char* sFileName) :CFile(sFileName)
{
    
}


CConfigFile::~CConfigFile()
{

}


int CConfigFile::getFieldValue(const char* const sFieldName, char* sFieldValue)
{
    if (NULL == sFieldName || NULL == sFieldValue)
    {
        return RETURN_CODE_ERROR;
    }

    char sLine[MAX_LENGTH_LINE] = {0};
    char* pos = NULL;

    //�ҵ�����������
    while (RETURN_CODE_OK == readALine(sLine, MAX_LENGTH_LINE))
    {
        if (pos = strstr(sLine, sFieldName))
        {
            break;
        }
    }

    //��ȡ����ֵ
    int iFieldNameLength = 0;

    //����������
    iFieldNameLength = strnlen_s(sFieldName, MAX_LENGTH_LINE);
    pos = pos + iFieldNameLength;

    //�����Ⱥ�
    while ((*pos) != 0 && (*pos++) != '=') { ; }

    //�����Ⱥź�Ŀո��Tab��
    while ((*(pos)) == ' ' && (*(pos)) != '\t' && (*pos++) != 0) { ; }

    //��ʼ��ȡ����ֵ
    int iValueLength = 0;
    char* posBegin = pos;
    while ((*pos) != 0 && (*(pos)) != ' ' && (*(pos)) != '\t' && (*(pos++)) != '\n')
    {
        iValueLength++;
    }
    strncpy_s(sFieldValue, iValueLength+1, posBegin, iValueLength);
    return RETURN_CODE_OK;
}