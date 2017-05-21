/*base64.h*/

#ifndef mimeb64H
#define mimeb64H

//---------------------------------------------------------------------------

// 把0~63转换为Base64码
char Chr2Base( char c );

// 把Base64码转换为0~63
char Base2Chr( char n );

// 对输入的字符串进行Base64编码,编码后的存储空间由调用者负责
void Base64Encode( const char* sIn , int iInLen , char* sOut , int& iOutLen );

// 对输入的字符串进行Base64解码,解码成功返回0,失败返回1,解码后的存储空间由调用者负责
void Base64Decode( const char* sIn , int iInLen , char* sOut , int& iOutLen );

char Hex2Chr( char n );
char Chr2Hex( char c );

//---------------------------------------------------------------------------
#endif


