/*base64.h*/

#ifndef mimeb64H
#define mimeb64H

//---------------------------------------------------------------------------

// ��0~63ת��ΪBase64��
char Chr2Base( char c );

// ��Base64��ת��Ϊ0~63
char Base2Chr( char n );

// ��������ַ�������Base64����,�����Ĵ洢�ռ��ɵ����߸���
void Base64Encode( const char* sIn , int iInLen , char* sOut , int& iOutLen );

// ��������ַ�������Base64����,����ɹ�����0,ʧ�ܷ���1,�����Ĵ洢�ռ��ɵ����߸���
void Base64Decode( const char* sIn , int iInLen , char* sOut , int& iOutLen );

char Hex2Chr( char n );
char Chr2Hex( char c );

//---------------------------------------------------------------------------
#endif


