//---------------------------------------------------------------------
//程序初始化文件访问类
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

    //找到参数所在行
    while (RETURN_CODE_OK == readALine(sLine, MAX_LENGTH_LINE))
    {
        if (pos = strstr(sLine, sFieldName))
        {
            break;
        }
    }

    //读取参数值
    int iFieldNameLength = 0;

    //跳过参数名
    iFieldNameLength = strnlen_s(sFieldName, MAX_LENGTH_LINE);
    pos = pos + iFieldNameLength;

    //跳过等号
    while ((*pos) != 0 && (*pos++) != '=') { ; }

    //跳过等号后的空格和Tab键
    while ((*(pos)) == ' ' && (*(pos)) != '\t' && (*pos++) != 0) { ; }

    //开始读取参数值
    int iValueLength = 0;
    char* posBegin = pos;
    while ((*pos) != 0 && (*(pos)) != ' ' && (*(pos)) != '\t' && (*(pos++)) != '\n')
    {
        iValueLength++;
    }
    strncpy_s(sFieldValue, iValueLength+1, posBegin, iValueLength);
    return RETURN_CODE_OK;
}