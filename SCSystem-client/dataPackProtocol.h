#ifndef DATAPACKPROTOCOL_H
#define DATAPACKPROTOCOL_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "public.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define CTRL_DIR BIT7

//这里char一个字节，word两个字节，three三个字节，long四个字节
#define CHAR2WORD(ptr)          (unsigned short)(((unsigned short)*((unsigned char*)(ptr)+1)<<8)|(unsigned short)*(unsigned char*)(ptr))
#define WORD2CHAR(ptr,val)      *(unsigned char*)(ptr)=(unsigned char)(val); *((unsigned char*)(ptr)+1)=(unsigned char)((unsigned short)(val)>>8)

#define CHAR2LONG(ptr)          (unsigned long)(((unsigned long)*((unsigned char*)(ptr)+3)<<24)|((unsigned long)*((unsigned char*)(ptr)+2)<<16)|((unsigned long)*((unsigned char*)(ptr)+1)<<8)|*(unsigned char*)(ptr))
#define LONG2CHAR(ptr,val)      *(unsigned char*)(ptr)=(unsigned char)(val); *((unsigned char*)(ptr)+1)=(unsigned char)((unsigned long)(val)>>8); *((unsigned char*)(ptr)+2)=(unsigned char)((unsigned long)(val)>>16); *((unsigned char*)(ptr)+3)=(unsigned char)((unsigned long)(val)>>24)

#define CHAR2THREE(ptr)         (unsigned long)(((unsigned long)*((unsigned char*)(ptr)+2)<<16)|((unsigned long)*((unsigned char*)(ptr)+1)<<8)|*(unsigned char*)(ptr))
#define THREE2CHAR(ptr,val)     *(unsigned char*)(ptr)=(unsigned char)(val); *((unsigned char*)(ptr)+1)=(unsigned char)((unsigned long)(val)>>8); *((unsigned char*)(ptr)+2)=(unsigned char)((unsigned long)(val)>>16);

#define CHAR2INT(ptr)           (int)(((unsigned long)*((unsigned char*)(ptr)+3)<<24)|((unsigned long)*((unsigned char*)(ptr)+2)<<16)|((unsigned long)*((unsigned char*)(ptr)+1)<<8)|*(unsigned char*)(ptr))
#define INT2CHAR(ptr,val)       *(unsigned char*)(ptr)=(unsigned char)(val); *((unsigned char*)(ptr)+1)=(unsigned char)((int)(val)>>8); *((unsigned char*)(ptr)+2)=(unsigned char)((int)(val)>>16); *((unsigned char*)(ptr)+3)=(unsigned char)((int)(val)>>24)

#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define IS_Minus(c)  ((c == '-'))

#define ISVALIDALPHA(c)  (IS_AF(c) || IS_af(c))
#define ISVALIDHEX(c)  (IS_AF(c) || IS_af(c) || IS_09(c))
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')
#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

//10进制转16进制
//15=0x15
extern unsigned char Int10ToHex16(unsigned int num);

//16进制数转10进制数
//0x15=15
extern int Hex16ToInt10(unsigned char ch);

//计算校验和，累加和
//p为开始字节的位置,len=所有字节数
extern unsigned char CalHexCS(unsigned char *pdata, unsigned int len);

//计算校验和，累加和
//p为开始字节的位置,len=所有字节数
extern unsigned char CalCS(unsigned char *p, unsigned int len);

//计算校验和，累加和
//p为开始字节的位置,len=所有字节数
extern unsigned int CalU32CS(unsigned char *p, unsigned int len);

//函 数 名：CharToASC()
//输    入：16进制
//返    回：ASCII字符
extern unsigned char CharToASC(unsigned char bHex);

//函 数 名：ASCToChar()
//输    入：ASCII字符
//返    回：16进制
extern unsigned char ASCToChar(unsigned char asc);

//0x12->"12"
//len=char长度,ascii长度=len*2
extern int CharP2ASC(unsigned char *pascii, unsigned char *pchar, int len);

//acsii转化为char
//2个ascii码转为1个字符(eg."12"->0x12)
//len=ascii长度
extern int ASCP2Char(unsigned char *pasc, unsigned char *pchar, int len);

//字符串转换整型
extern int Str2Int(unsigned char *inputstr, int *intnum);

//高低位相反
extern unsigned char CharBitAllChange(unsigned char ch);

//高低位相反
extern void BufBitAllChange(unsigned char *ch, unsigned int datalen);

//检查IP地址类型
extern unsigned char CheckAddrType(unsigned char *pdata, unsigned int len);

#define RECV_DATA_TYPE_SHO              (0)
#define RECV_DATA_TYPE_ERROR			(1)
#define RECV_DATA_TYPE_OK				(2)
#define RECV_DATA_TYPE_STRING			(3)

#define RETURN_CONTINUE_RECV			(1)//接收的数据不完整
#define RETURN_FRAME_OK					(2)//接收到一个完整的帧
#define RETURN_FRAME_ERROR				(3)//接收的帧错误

enum
{
    ASCII=0,
    BINARY
};

enum
{
    PC_TO_ARM=0x00,
    ARM_TO_PC=0x80
};

#define SOH_VALUE (unsigned long)('S'+'O'*0x100+'H'*0x10000+' '*0x1000000)//0x20484F53

#define SOH_START_ADDRESS                   (0)
#define SOH_LEN                             (4)

#define DATALEN_START_ADDRESS               (4)
#define DATALEN_LEN                         (4)

#define COMMAND_ID_START_ADDRESS            (8)
#define COMMAND_ID_LEN                      (2)

#define DATA_TYPE_START_ADDRESS             (10)
#define DATA_TYPE_LEN                       (1)

#define CTRL_START_ADDRESS                  (11)
#define CTRL_LEN                            (1)

#define DATA_START_ADDRESS                  (12)
#define CS_LEN                              (2)

#define CS_VERIFY_FIX_LEN                   (SOH_LEN+DATALEN_LEN+COMMAND_ID_LEN+DATA_TYPE_LEN+CTRL_LEN)
#define DATA_PACK_FIX_LEN                   (SOH_LEN+DATALEN_LEN+COMMAND_ID_LEN+DATA_TYPE_LEN+CTRL_LEN+CS_LEN)

//解析数据包的
#define UPLINK_PROTOL_GET_SOH(pstart)                   (CHAR2LONG(pstart))//设置命令开始码
#define UPLINK_PROTOL_GET_LEN(pstart)           		(CHAR2LONG(&pstart[DATALEN_START_ADDRESS]))
#define UPLINK_PROTOL_GET_CMD_ID(pstart)                        (CHAR2WORD(&pstart[COMMAND_ID_START_ADDRESS]))
#define UPLINK_PROTOL_GET_TYPE(pstart)                          (pstart[DATA_TYPE_START_ADDRESS])
#define UPLINK_PROTOL_GET_CTRL(pstart)                          (pstart[CTRL_START_ADDRESS])
#define UPLINK_PROTOL_GET_DATA(pstart,dataBuf,dataLen)          (memcpy(dataBuf,pstart+DATA_START_ADDRESS,dataLen))
#define UPLINK_PROTOL_GET_CS(pstart)                            (CHAR2WORD(pstart+CS_VERIFY_FIX_LEN+UPLINK_PROTOL_GET_LEN(pstart)))
//组建数据包的宏定义
#define UPLINK_PROTOL_SET_SOH(pstart)                           (memcpy(pstart,"SOH ",SOH_LEN))//设置命令开始码
#define UPLINK_PROTOL_SET_LEN(pstart,dataLen)      		LONG2CHAR(&pstart[DATALEN_START_ADDRESS], dataLen)//设置命令长度
#define UPLINK_PROTOL_SET_CMD_ID(pstart,commandId)              WORD2CHAR(&pstart[COMMAND_ID_START_ADDRESS], commandId)//命令ID
#define UPLINK_PROTOL_SET_TYPE(pstart,type)       		(pstart[DATA_TYPE_START_ADDRESS] = type)//数据类型
#define UPLINK_PROTOL_SET_CTRL(pstart,ctrl)     		(pstart[CTRL_START_ADDRESS] |= (ctrl&CTRL_DIR))//控制字
#define UPLINK_PROTOL_SET_RSEQ(pstart,pseq)     		(pstart[CTRL_START_ADDRESS] |= (pseq&0x0f))//命令帧序列号
#define UPLINK_PROTOL_SET_DATA(pstart,pdata,dtlen)              (memcpy(pstart+DATA_START_ADDRESS,pdata,dtlen))
#define UPLINK_PROTOL_SET_CS(pstart,cs)                         WORD2CHAR(pstart+DATA_START_ADDRESS+UPLINK_PROTOL_GET_LEN(pstart),cs)

extern S32 protolDataCreate(UCHAR *psend, U16 commandId, U08 dataType, U08 dataDir, U08 dataSeq, UCHAR *dataBuf, U16 datalen);
extern U08 protolDataParse(UCHAR *dataBuf, int dataLen, dataFrameContent *p);
extern int recvDataParser(unsigned char *sourceBuf, unsigned char *buf, unsigned int *len, unsigned int *allLen);
#endif // DATAPACKPROTOCOL_H

