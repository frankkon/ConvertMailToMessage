//---------------------------------------------------------------------
//终端类型的父类声明文件
//---------------------------------------------------------------------
#ifndef _TEMINAL_H
#define _TEMINAL_H

class CTeminal
{
public:
    CTeminal();
    virtual ~CTeminal();

    virtual void showMessage() = 0;

private:

};

#endif //_TEMINAL_H
