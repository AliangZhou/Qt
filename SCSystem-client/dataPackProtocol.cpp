#include "dataPackProtocol.h"
/***************************************************************************/
// 10进制转16进制
//15=0x15
extern unsigned char Int10ToHex16(unsigned int num)
{
    unsigned char result = 0x00;
    result = 16 * (num / 10) + num % 10;
    return result;
}
/***************************************************************************/
// 16进制数转10进制数
//0x15=15
extern int Hex16ToInt10(unsigned char ch)
{
    int result = 0;
    result = 10 * ((ch) / 16) + (ch) % 16;
    return result;
}
/***************************************************************************/
extern unsigned char CalHexCS(unsigned char *pdata, unsigned int len)
{
    unsigned char cs = 0;
    unsigned int i = 0;
    for (i = 0; i<len; i++)
    {
        cs = cs + *pdata++;
    }
    cs = ~cs;
    cs = cs + 0x01;
    return cs;
}
/***************************************************************************/
// 计算校验和，累加和
//p为开始字节的位置,len=所有字节数
extern unsigned char CalCS(unsigned char *p, unsigned int len)
{
    unsigned char result = 0x00;
    unsigned int i = 0;
    for (i = 0; i<len; i++)
    {
        result += p[i];
    }
    return result;
}
/***************************************************************************/
extern unsigned int CalU32CS(unsigned char *p, unsigned int len)
{
    unsigned int result = 0x00;
    unsigned int i = 0;
    for (i = 0; i<len; i++)
    {
        result += p[i];
    }
    return result;
}
/***************************************************************************/
//函 数 名：CharToASC()
//输    入：16进制
//返    回：ASCII字符
/////////////////////////////////////////////////////////////////////
extern unsigned char CharToASC(unsigned char bHex)
{
    if ((bHex <= 9)) bHex += 0x30;
    else if ((bHex >= 10) && (bHex <= 15))//大写字母
        bHex += 0x37;
    else bHex = 0xff;
    return bHex;
}
/***************************************************************************/
/////////////////////////////////////////////////////////////////////
//函 数 名：ASCToChar()
//输    入：ASCII字符
//返    回：16进制
/////////////////////////////////////////////////////////////////////
extern unsigned char ASCToChar(unsigned char asc)
{
    if ((asc >= 0x30) && (asc <= 0x39))
        asc -= 0x30;
    else if ((asc >= 0x41) && (asc <= 0x46))//大写字母
        asc -= 0x37;
    else if ((asc >= 0x61) && (asc <= 0x66))//小写字母
        asc -= 0x57;
    else asc = 0xff;
    return asc;
}
/***************************************************************************/
//0x12->"12"
//len=char长度,ascii长度=len*2
extern int CharP2ASC(unsigned char *pascii, unsigned char *pchar, int len)
{
    int i;
    unsigned char temp = 0;
    for (i = 0; i<len; i++)
    {
        temp = CharToASC((*pchar >> 4) & 0x0f);
        *pascii++ = temp;
        temp = CharToASC((*pchar) & 0x0f);
        *pascii++ = temp;
        pchar++;
    }
    return 0;
}
/***************************************************************************/
//acsii转化为char
// 2个ascii码转为1个字符(eg."12"->0x12)
//len=ascii长度
extern int ASCP2Char(unsigned char *pasc, unsigned char *pchar, int len)
{
    int i, thislen;
    unsigned char temp = 0, tchar = 0;
    thislen = len / 2;
    for (i = 0; i<thislen; i++)
    {
        temp = ASCToChar(*pasc++);
        temp = temp << 4;
        tchar = temp & 0xf0;
        temp = ASCToChar(*pasc++);
        temp = temp & 0x0f;
        (*pchar) = tchar | temp;
        pchar++;
    }
    return thislen;
}
/***************************************************************************/
/**
* @brief  Convert a string to an integer
* @param  inputstr: The string to be converted
* @param  intnum: The intger value
* @retval 1: Correct
*         0: Error
*/
extern int Str2Int(unsigned char *inputstr, int *intnum)
{
    int value = 0;

    /* over 8 digit hex --invalid */
    if (strlen((const char *)inputstr) > 12)
        return 1;

    if ((strlen((const char *)inputstr) > 2) && (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X')))
    {
        inputstr += 2;
        while (*inputstr && (!((*inputstr) == '\r' || (*inputstr) == '\n')))
        {
            if (!ISVALIDHEX(*inputstr))
                return 1;

            value = (value << 4) + CONVERTHEX(*inputstr);
            inputstr++;
        }
    }
    else
    {
        while (*inputstr && (!((*inputstr) == '\r' || (*inputstr) == '\n')))
        {
            if (!IS_09(*inputstr))
                return 1;

            value = value * 10 + CONVERTDEC(*inputstr);
            inputstr++;
        }
    }
    *intnum = value;
    return 0;
}
/***************************************************************************/
//高低位相反
extern unsigned char CharBitAllChange(unsigned char ch)
{
    int i;
    unsigned char newch = 0;
    for (i = 0; i<8; i++)
    {
        newch = newch << 1;
        if (ch & 0x01)
        {
            newch |= 0x01;
        }
        ch = ch >> 1;

    }
    return newch;
}
/***************************************************************************/
//高低位相反
extern void BufBitAllChange(unsigned char *ch, unsigned int datalen)
{
    unsigned int i;
    for (i = 0; i<datalen; i++)
    {
        *ch = CharBitAllChange(*ch);
        ch++;
    }
}
/***************************************************************************/
//检查IP地址类型
extern unsigned char CheckAddrType(unsigned char *pdata, unsigned int len)
{
    unsigned int i, addrflag = 0;
    for (i = 0; i<len; i++)
    {
        if (pdata[i] == 0xff)//全广播地址
        {
            addrflag += 1;
        }
        else if (pdata[i] == 0xfe)//0xfe地址
        {
            addrflag += 8;
        }
        else if (pdata[i] == 0xfd)//0xfd地址
        {
            addrflag += 16;
        }
        else if (pdata[i] == 0)//0x00地址
        {
            addrflag += 24;
        }
        else addrflag = 0;
    }
    if (addrflag == len * 1)//广播地址
    {
        return 1;
    }
    else if (addrflag == len * 8)//输入/输出板地址
    {
        return 2;
    }
    else if (addrflag == len * 16)//输入/输出板地址
    {
        return 3;
    }
    else if (addrflag == len * 24)//片选地址
    {
        return 4;
    }
    return 0;
}
/***************************************************************************/
extern S32 protolDataCreate(UCHAR *psend, U16 commandId, U08 dataType, U08 dataDir, U08 dataSeq, UCHAR *dataBuf, U16 datalen)
{
    U32 cs = 0;
    UPLINK_PROTOL_SET_SOH(psend);//设置数据包头
    UPLINK_PROTOL_SET_LEN(psend,datalen);//设置数据长度
    UPLINK_PROTOL_SET_CMD_ID(psend,commandId);//设置命令ID
    UPLINK_PROTOL_SET_TYPE(psend,dataType);//设置数据类型
    UPLINK_PROTOL_SET_CTRL(psend,dataDir);//设置数据发送方向
    UPLINK_PROTOL_SET_RSEQ(psend,dataSeq);//设置数据帧序列
    UPLINK_PROTOL_SET_DATA(psend,dataBuf,datalen);//设置数据
    cs = CalU32CS(psend, CS_VERIFY_FIX_LEN+datalen);//求数据帧的CS值
    UPLINK_PROTOL_SET_CS(psend,cs);//设置数据帧的CS值
    return DATA_PACK_FIX_LEN + datalen;
}
/***************************************************************************/
extern int recvDataParser(unsigned char *sourceBuf, unsigned char *buf, unsigned int *len, unsigned int *allLen)
{
    unsigned char SOH_CHAR[]={0x53,0x4F,0x48,0x20};
    unsigned int j=0,okFlag=0,datalen=0,copyDataLen=0,surplusLen=0;
    unsigned char *pStart = sourceBuf;
    unsigned char buffer[4096]={0};
    #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
    printf("recvDataParser is...\r\n");
    datalen = *allLen;
    for(j = 0; j < datalen; j++)
    {
            printf("%02x ", sourceBuf[j]);
    }
    printf("\r\n");
    #endif
    //判断接收的数据前四个字节是否为"SOH "
    for (j = 0;j < sizeof(SOH_CHAR);j++)
    {
            if (pStart[j] == SOH_CHAR[j])
            {
                    okFlag++;
            }
    }
    if (okFlag != sizeof(SOH_CHAR))//接收到的数据不是以"SOH "开头的,帧包头不完整，则继续接收数据
    {
        return RETURN_CONTINUE_RECV;
    }
    else//二进制数据包
    {
        if(*allLen < DATA_PACK_FIX_LEN)//幁格式不完整，则继续接收数据
        {
            return RETURN_CONTINUE_RECV;
        }
        else
        {
            datalen = CHAR2LONG(&pStart[4]);
            copyDataLen = datalen + DATA_PACK_FIX_LEN;
            if(copyDataLen > *allLen)
            {
                if(copyDataLen>=TCP_DATA_MAX_LEN)
                {
                    *allLen = 0;//表示接收数据缓冲区数据发生错误，清掉缓冲区所有数据
                    return RETURN_FRAME_ERROR;
                }
                else//数据没有接收完，继续接收数据
                {
                    return RETURN_CONTINUE_RECV;
                }
            }
            else//二进制一帧数据接收完成
            {
                #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
                qDebug()<<"the copyDataLen is " << copyDataLen;
				#endif
                memcpy(buf,sourceBuf,copyDataLen);
                *len=copyDataLen;
                surplusLen=*allLen-copyDataLen;
                *allLen=surplusLen;
                memcpy(buffer,&sourceBuf[copyDataLen],*allLen);
                memcpy(sourceBuf,buffer,*allLen);
                #if 0
                printf("...................%d...................\r\n", *allLen);
                #endif
                return RETURN_FRAME_OK;
            }
        }
    }
}
/***************************************************************************/
extern U08 protolDataParse(UCHAR *dataBuf,int dataLen, dataFrameContent *p)
{
    dataFrameContent Obj;
    U32 cs = 0;
	#ifdef DEBUG_COMMON_INFORMATION_DISPLAY
    printf("the SOH_VALUE is %ld...%ld\n",SOH_VALUE,UPLINK_PROTOL_GET_SOH(dataBuf));
	#endif
    if(SOH_VALUE!=UPLINK_PROTOL_GET_SOH(dataBuf))//数据包头不符合
    {
    	#ifdef DEBUG_ERROR_INFORMATION_DISPLAY
        printf("the SOH error!!!\n");
		#endif
        return 0;
    }
    cs = CalU32CS(dataBuf, dataLen - CS_LEN );//求数据帧的CS值
    cs = cs & 0x0000FFFF;
	#ifdef DEBUG_COMMON_INFORMATION_DISPLAY
    printf("the cs is %0x...%0x\n",cs,UPLINK_PROTOL_GET_CS(dataBuf));
	#endif
    if(cs!=UPLINK_PROTOL_GET_CS(dataBuf))
    {
    	#ifdef DEBUG_ERROR_INFORMATION_DISPLAY
        printf("the cs error!!!\n");
		#endif
        return 0;
    }
    if(dataLen!=(UPLINK_PROTOL_GET_LEN(dataBuf)+DATA_PACK_FIX_LEN))
    {
    	#ifdef DEBUG_ERROR_INFORMATION_DISPLAY
        printf("the datalen error!!!\n");
		#endif
        return 0;
    }
    Obj.commandId = UPLINK_PROTOL_GET_CMD_ID(dataBuf);
    Obj.dataType = UPLINK_PROTOL_GET_TYPE(dataBuf);
    Obj.datalen = UPLINK_PROTOL_GET_LEN(dataBuf);
    UPLINK_PROTOL_GET_DATA(dataBuf, Obj.data, Obj.datalen);
    memcpy(p,&Obj,sizeof(dataFrameContent));
    return 1;
}
/***************************************************************************/

