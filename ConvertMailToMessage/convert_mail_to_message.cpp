//---------------------------------------------------------------------
//������ģ��cpp�ļ�
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

    //��ȡ�����ļ�

    //���ö�ȡ����Ϣͨ��POP3Э���ȡ�ʼ�

    //���ʼ�����ͨ�����ŷ��͵�ָ���ֻ�

    return RETURN_CODE_OK;

#endif
}


