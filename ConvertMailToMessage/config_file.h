//---------------------------------------------------------------------
//�����ʼ���ļ�������
//---------------------------------------------------------------------
#ifndef _CONFIG_FILE_H
#define _CONFIG_FILE_H

#include "file.h"

class CConfigFile : public CFile
{
private:
    //��Ա���������ڴˡ�

public:
    //��Ա���������ڴˡ�
    CConfigFile();
    CConfigFile(const char* sFileName);
    virtual ~CConfigFile();

    int getFieldValue(const char* const sFieldName, char* sFieldValue);

};


#endif //_CONFIG_FILE_H
