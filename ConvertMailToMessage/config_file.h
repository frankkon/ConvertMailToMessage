//---------------------------------------------------------------------
//程序初始化文件访问类
//---------------------------------------------------------------------
#ifndef _CONFIG_FILE_H
#define _CONFIG_FILE_H

#include "file.h"

class CConfigFile : public CFile
{
private:
    //成员变量定义于此。

public:
    //成员函数定义于此。
    CConfigFile();
    CConfigFile(const char* sFileName);
    virtual ~CConfigFile();

    int getFieldValue(const char* const sFieldName, char* sFieldValue);

};


#endif //_CONFIG_FILE_H
