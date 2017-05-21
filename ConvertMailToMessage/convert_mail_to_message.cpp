//---------------------------------------------------------------------
//主处理模块cpp文件
//---------------------------------------------------------------------
#include "convert_mail_to_message.h"

#ifdef _UNIT_TEST
#include "unit_test.h"
#endif

using namespace std;

int main(int argc, char** argv)
{
#ifdef _UNIT_TEST
    return unitTest();
#else

    //读取配置文件

    //利用读取的信息通过POP3协议获取邮件

    //将邮件内容通过短信发送到指定手机

    return RETURN_CODE_OK;

#endif
}


