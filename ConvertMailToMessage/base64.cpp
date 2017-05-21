/*base64*/

#include "base64.h"
#include <cctype>
#include <cstring>


// 4bit binary to char 0-F
char Hex2Chr( char n )
{
    n &= 0xF;
    if ( n < 10 )
        return ( char )( n + '0' );
    else
        return ( char )( n - 10 + 'A' );
}
//---------------------------------------------------------------------------
// char 0-F to 4bit binary
char Chr2Hex( char c )
{
    c = ( char )toupper( c );
    if ( c >= '0' && c <= '9' )
        return ( char )( c - '0' );
    else if ( c >= 'A' && c <= 'F' )
        return ( char )( c - 'A' + 10 );
    else
        return -1;
}

//---------------------------------------------------------------------------
// Base64 code table
// 0-63 : A-Z(25) a-z(51), 0-9(61), +(62), /(63)
char Chr2Base( char n )
{ 
// ��0~63ת��ΪBase64��
    
    // ֻȡ����λ
    n &= 0x3F; // n����0~63
    if ( n < 26 ) // 26����д��ĸ
        return ( char )( n + 'A' );
    else if ( n < 52 ) // ��26��Сд��ĸ
        return ( char )( n - 26 + 'a' );
    else if ( n < 62 )  // ��10��������ĸ
        return ( char )( n - 52 + '0' );
    else if ( n == 62 )
        return '+';
    else  // n == 63
        return '/';
}
//---------------------------------------------------------------------------
char Base2Chr( char c )
{
// ��Base64��ת��Ϊ0~63
// ����ֵ��0~63֮���ʾ������Base64��,�����������

    if ( c >= 'A' && c <= 'Z' )
        return ( char )( c - 'A' );  // ��0~26(������25)֮��
    else if ( c >= 'a' && c <= 'z' )
        return ( char )( c - 'a' + 26 ); // ��26~52(������52)֮��
    else if ( c >= '0' && c <= '9' )
        return ( char )( c - '0' + 52 ); // ��52~62(������62)֮��
    else if ( c == '+' )
        return 62;    
    else 
        return 63;  // '/'���Base64��

}
//---------------------------------------------------------------------------
//
void Base64Encode( const char* sIn , int iInLen , char* sOut , int& iOutLen )
{
/* �������ַ�������Base64����
 * ���ַ���ÿ����Byte�ضϳ�Ϊ4 ������,��λ����,ÿ����6 λ�ַ���һ
 * ��Byte��,ÿByte�ĸ߶�λ��Ч. 
 * ��Ч��6 λ�������64 ���ַ����±�ʾ:
 * "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/".
 * ��������ʣ��һ���������ֽ��޷����������ֽ�,��ô������õȺ�'='����.
 * ����˵��:    sIn:��������ַ���,iInLen:��������ַ�������
 *              sOut:�����Ĵ洢��ַ,iOutLen:�������ַ�������
 */
    int n = iInLen;
    char c, t,temp;
    char asTemp[2];
    int i;
    
    iOutLen = 0;
    t = 0;
    
    for (  i = 1; i <= n; i++ )
    {
        c = sIn[i-1];
        if ( i % 3 == 1 )
        {
            // ȡc�ĸ���λ,�ٰ����Ƶ�����λ����,��������:00XXXXXX
            temp = ( char )( (c >> 2) & 0x3f );
            asTemp[0] = Chr2Base(temp);
            memcpy( sOut+iOutLen , asTemp , 1 );
            iOutLen++;
            t = ( char )( ( c << 4 ) & 0x3F );  // ȡʣ�µĵͶ�λ,��������:00XX0000
        }
        else if ( i % 3 == 2 )
        {
            // ȡc�ĸ���λ,�ٰ����Ƶ�����λ����,��������:0000XXXX
            temp = ( char )( (c >> 4) & 0x0f );            
            asTemp[0] = Chr2Base(( char )( t | temp ));
            memcpy( sOut+iOutLen , asTemp , 1 );
            iOutLen++;
            t = ( char )( ( c << 2 ) & 0x3F ); // ȡʣ�µĵ���λ,��������:00XXXX00
        }
        else
        {
            // ȡc�ĸ߶�λ,�ٰ����Ƶ��Ͷ�λ����,��������:000000XX
            temp = ( char )( (c >> 6) & 0x03 );            
            asTemp[0] = Chr2Base(( char )( t | temp ));
            // ��ʣ�µĵ���λ���б���
            asTemp[1] = Chr2Base( c );
            memcpy( sOut+iOutLen , asTemp , 2 );
            iOutLen+=2;
        }
    }
    if ( n % 3 != 0 )
    {
        // ��ʣ�µ�һ���ӷ����б���
        asTemp[0] = Chr2Base( t );
        memcpy( sOut+iOutLen , asTemp , 1 );
        iOutLen++;
            
        if ( n % 3 == 1 )
        {
            // �õȺ�'='����
            asTemp[0] = '=';
            asTemp[1] = '=';
            memcpy( sOut+iOutLen , asTemp , 2 );
            iOutLen+=2;
        }
        else
        {
            // �õȺ�'='����
            asTemp[0] = '=';
            memcpy( sOut+iOutLen , asTemp , 1 );
            iOutLen++;            
        }
    }
    sOut[iOutLen] = 0; // ���Ͻ�����NULL
}
//---------------------------------------------------------------------------
//
void Base64Decode( const char* sIn , int iInLen , char* sOut , int& iOutLen )
{
/* �������ַ�������Base64����
 * ���ĸ�Byte��ȡÿ��Byte�ĺ�6 bit,��ϳ�����Byte
 * ����˵��:    sIn:��������ַ���,iInLen:��������ַ�������
 *              sOut:�����Ĵ洢��ַ,iOutLen:�������ַ���
 * ����ֵ˵��: ����ɹ�����0,ʧ�ܷ���1
 */
    int n = iInLen;
    char c, t,temp;
    char asTemp[2];
    int i;

    iOutLen = 0;
    t = 0;

    for ( i = 1; i <= n; i++ )
    {
        if ( sIn[i-1] == '=' )
        {
            break; // ��������ĵȺ�'='
        }
        // ת��Ϊ0~63
        c = Base2Chr( sIn[i-1] );

        if ( i % 4 == 1 )
        {
            t = ( char )( c << 2 ); // ȡ����λ,��������:XXXXXX00
        }
        else if ( i % 4 == 2 )
        {
            // ȡc�ĸ���λ,�ٰ����Ƶ�����λ����,��������:0000XXXX
            temp = ( char )( (c >> 4) & 0x0f );            
            asTemp[0] = ( char )( t | temp );
            memcpy( sOut+iOutLen, asTemp, 1);
            iOutLen ++;
            t = ( char )( c << 4 ); // ȡ����λ,��������:XXXX0000
        }
        else if ( i % 4 == 3 )
        {
            // ȡc�ĸ���λ,�ٰ����Ƶ�����λ����,��������:00XXXXXX
            temp = ( char )( (c >> 2) & 0x3f );
            asTemp[0] = ( char )( t | temp );
            memcpy( sOut+iOutLen, asTemp, 1);
            iOutLen ++;
            t = ( char )( c << 6 ); // ȡ�Ͷ�λ,��������:XX000000
        }
        else
        {
            // ��һ���ֽ����µĶ�λ��c�ĵ���λ��ϳ�һ��Byte
            asTemp[0] = ( char )( t | c );
            memcpy( sOut+iOutLen, asTemp, 1);
            iOutLen ++;
        }
    }       
}
//---------------------------------------------------------------------------


