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
// 把0~63转换为Base64码
    
    // 只取低六位
    n &= 0x3F; // n属于0~63
    if ( n < 26 ) // 26个大写字母
        return ( char )( n + 'A' );
    else if ( n < 52 ) // 加26个小写字母
        return ( char )( n - 26 + 'a' );
    else if ( n < 62 )  // 加10个数字字母
        return ( char )( n - 52 + '0' );
    else if ( n == 62 )
        return '+';
    else  // n == 63
        return '/';
}
//---------------------------------------------------------------------------
char Base2Chr( char c )
{
// 把Base64码转换为0~63
// 返回值在0~63之间表示输入是Base64码,否则输入错误

    if ( c >= 'A' && c <= 'Z' )
        return ( char )( c - 'A' );  // 在0~26(不包括25)之间
    else if ( c >= 'a' && c <= 'z' )
        return ( char )( c - 'a' + 26 ); // 在26~52(不包括52)之间
    else if ( c >= '0' && c <= '9' )
        return ( char )( c - '0' + 52 ); // 在52~62(不包括62)之间
    else if ( c == '+' )
        return 62;    
    else 
        return 63;  // '/'与非Base64码

}
//---------------------------------------------------------------------------
//
void Base64Encode( const char* sIn , int iInLen , char* sOut , int& iOutLen )
{
/* 对输入字符流进行Base64编码
 * 将字符流每三个Byte截断成为4 个部分,高位在先,每部分6 位分放在一
 * 个Byte中,每Byte的高二位无效. 
 * 有效的6 位用下面的64 个字符重新表示:
 * "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/".
 * 如果编码后剩有一个或两个字节无法构成三个字节,那么输出将用等号'='补足.
 * 参数说明:    sIn:待编码的字符串,iInLen:待编码的字符串长度
 *              sOut:编码后的存储地址,iOutLen:编码后的字符串长度
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
            // 取c的高六位,再把它移到低六位放置,其存放形如:00XXXXXX
            temp = ( char )( (c >> 2) & 0x3f );
            asTemp[0] = Chr2Base(temp);
            memcpy( sOut+iOutLen , asTemp , 1 );
            iOutLen++;
            t = ( char )( ( c << 4 ) & 0x3F );  // 取剩下的低二位,其存放形如:00XX0000
        }
        else if ( i % 3 == 2 )
        {
            // 取c的高四位,再把它移到低四位放置,其存放形如:0000XXXX
            temp = ( char )( (c >> 4) & 0x0f );            
            asTemp[0] = Chr2Base(( char )( t | temp ));
            memcpy( sOut+iOutLen , asTemp , 1 );
            iOutLen++;
            t = ( char )( ( c << 2 ) & 0x3F ); // 取剩下的低四位,其存放形如:00XXXX00
        }
        else
        {
            // 取c的高二位,再把它移到低二位放置,其存放形如:000000XX
            temp = ( char )( (c >> 6) & 0x03 );            
            asTemp[0] = Chr2Base(( char )( t | temp ));
            // 对剩下的低六位进行编码
            asTemp[1] = Chr2Base( c );
            memcpy( sOut+iOutLen , asTemp , 2 );
            iOutLen+=2;
        }
    }
    if ( n % 3 != 0 )
    {
        // 对剩下的一个子符进行编码
        asTemp[0] = Chr2Base( t );
        memcpy( sOut+iOutLen , asTemp , 1 );
        iOutLen++;
            
        if ( n % 3 == 1 )
        {
            // 用等号'='补足
            asTemp[0] = '=';
            asTemp[1] = '=';
            memcpy( sOut+iOutLen , asTemp , 2 );
            iOutLen+=2;
        }
        else
        {
            // 用等号'='补足
            asTemp[0] = '=';
            memcpy( sOut+iOutLen , asTemp , 1 );
            iOutLen++;            
        }
    }
    sOut[iOutLen] = 0; // 加上结束符NULL
}
//---------------------------------------------------------------------------
//
void Base64Decode( const char* sIn , int iInLen , char* sOut , int& iOutLen )
{
/* 对输入字符流进行Base64解码
 * 在四个Byte中取每个Byte的后6 bit,组合成三个Byte
 * 参数说明:    sIn:待解码的字符串,iInLen:待解码的字符串长度
 *              sOut:解码后的存储地址,iOutLen:解码后的字符串
 * 返回值说明: 解码成功返回0,失败返回1
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
            break; // 跳过补足的等号'='
        }
        // 转换为0~63
        c = Base2Chr( sIn[i-1] );

        if ( i % 4 == 1 )
        {
            t = ( char )( c << 2 ); // 取低六位,其存放形如:XXXXXX00
        }
        else if ( i % 4 == 2 )
        {
            // 取c的高四位,再把它移到低四位放置,其存放形如:0000XXXX
            temp = ( char )( (c >> 4) & 0x0f );            
            asTemp[0] = ( char )( t | temp );
            memcpy( sOut+iOutLen, asTemp, 1);
            iOutLen ++;
            t = ( char )( c << 4 ); // 取低四位,其存放形如:XXXX0000
        }
        else if ( i % 4 == 3 )
        {
            // 取c的高六位,再把它移到低六位放置,其存放形如:00XXXXXX
            temp = ( char )( (c >> 2) & 0x3f );
            asTemp[0] = ( char )( t | temp );
            memcpy( sOut+iOutLen, asTemp, 1);
            iOutLen ++;
            t = ( char )( c << 6 ); // 取低二位,其存放形如:XX000000
        }
        else
        {
            // 上一个字节留下的二位与c的低六位结合成一个Byte
            asTemp[0] = ( char )( t | c );
            memcpy( sOut+iOutLen, asTemp, 1);
            iOutLen ++;
        }
    }       
}
//---------------------------------------------------------------------------


