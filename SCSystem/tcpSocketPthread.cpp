/**********************************************************************************/
#include "tcpSocketPthread.h"
/**********************************************************************************/
tcpSocketThread::tcpSocketThread() : bWriteData(false)
{

}
/**********************************************************************************/
tcpSocketThread::~tcpSocketThread()
{

}

void tcpSocketThread::setWriteDataState(bool bWrite)
{
    bWriteData = bWrite;
}
/**********************************************************************************/
void tcpSocketThread::run()
{
    QString str;
    char buf[TCP_BUF_MAX_LEN] = {0};
    unsigned int ret = 0,frameLen=0,bufDatalen=0;
    msgId = -1;
    runFlag = 1;
    readBufIndex = 0;
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(ptr))//客户端的初始化
    {
        #ifdef DEBUG_ERROR_INFORMATION_DISPLAY
        qDebug() << "setSocketDescriptor error";
        #endif
    }

    if(socket->waitForConnected(10000))
    {
        #ifdef DEBUG_OK_INFORMATION_DISPLAY
        qDebug()<<"Connect Success";
        #endif
    }
    else
    {
        #ifdef DEBUG_ERROR_INFORMATION_DISPLAY
        qDebug()<<"Connect Fail";
        #endif
    }

    #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
    qDebug()<<"client Address:"<<socket->peerAddress();
    #endif

    QString ip = socket->peerAddress().toString();
    emit clientConnectStateChange(ip);//发送客户端连接信号
    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(sockRecvData()));//有数据来
    QObject::connect(socket,SIGNAL(disconnected()),this,SLOT(disconnectToHost()));
    QObject::connect(this,SIGNAL(finished()),this,SLOT(quitPthread()));

    while(true)
    {
        if(!runFlag)
        {
            break;
        }

        //完成需要的功能
        //从socket端口读取数据
        //尝试在waitForReadyRead()函数之前有数据过来，当执行wait函数时，会直接返回true
        //在阻塞期间来数据，会返回true
        //阻塞默认参数30000ms，超时返回false
        if(socket->waitForReadyRead(5))//实现了阻塞，等到有数据可读了或超时才返回
        {
#if 0
            str = QString::fromLocal8Bit(socket->readAll());
            if(!str.isEmpty())
            {
                qDebug() << "read Data from client len" << str.length() << str;
                emit sendReadData(str);
            }
#else
            ret = socket->read(buf,TCP_BUF_MAX_LEN);
            if((ret > 0)&&(ret < TCP_BUF_MAX_LEN))//读取到有效的数据
            {
                #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
                qDebug() << "read Data from client len" << ret;
//                for(unsigned int i = 0; i< ret; i++)
//                {
//                    qDebug("%02x ",buf[i]);
//                }
                #endif
                memcpy(&readBuf[readBufIndex],buf,ret);
                readBufIndex = readBufIndex + ret;
                bufDatalen=readBufIndex;
                while(1)
                {
                    memset(buf,0,sizeof(buf));
                    frameLen=0;
                    ret = recvDataParser(readBuf,(unsigned char*)buf,&frameLen,&bufDatalen);
                    qDebug()<<"the ret is....."<< ret << "the frameLen..." << frameLen;
                    if(RETURN_FRAME_OK==ret)//得到一个完整的命令帧
                    {
                        dataFrameContent Obj;
                        if(frameLen >= DATA_PACK_FIX_LEN)//完整的数据包
                        {
                            protolDataParse((unsigned char*)buf,frameLen,&Obj);
                            qDebug() << "Obj.commandId: " << Obj.commandId << "obj.dataType:" << Obj.dataType << "obj.datalen: "<< Obj.datalen
                                     << "msgId: " << msgId;
                            if(ASCII==Obj.dataType)
                            {
                                #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
                                printf("the recv data is %s",(char*)Obj.data);
                                #else
                                emit sendReadData((char*)Obj.data);
                                #endif
                            }
                            else
                            {

                                str = "";
                                for(int i = 0 ; i < Obj.datalen; i++)
                                {
//                                    #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
//                                    printf("%02x ",Obj.data[i]);
//                                    #else
                                    str.append(QString("%1").arg(Obj.data[i] & 0xFF, 2, 16, QLatin1Char('0')));
                                    str.append(" ");
                                    if((i!=0)&&(!(i%22)))
                                    {
                                        str.append("\n");
                                    }
//                                    #endif
                                }
                                qDebug() << "BINARY: " << (char*)Obj.data << str;
                                emit sendReadData(str);
                            }
                        }
                    }
                    else if(RETURN_FRAME_ERROR==ret)
                    {
                        break;
                    }
                    else if(RETURN_CONTINUE_RECV==ret)
                    {
                        break;
                    }
                    if(0==bufDatalen)//接收缓冲区没有数据
                    {
                        break;
                    }
                }
                readBufIndex = bufDatalen;
            }
#endif
        }
        //往socket端口写数据
        if(msgId >= 0)
        {
            socket->write((char*)writeBuf,writeLen);
            socket->flush();
            msgId = -1;
        }
        usleep(1000);
    }
}
/**********************************************************************************/
void tcpSocketThread::sockRecvData(void)
{
//完成需要的功能
#if 1
    QString str0 = "";
    str0.append(QString::fromLocal8Bit(socket->readAll()));
    qDebug() << "read Data from client ";
#else
    char buf[TCP_BUF_MAX_LEN] = {0};
    int ret = socket->read(buf,TCP_BUF_MAX_LEN);
    qDebug()<<"read ret........................"<<ret;
#endif
}
/**********************************************************************************/
void tcpSocketThread::recvTcpSendData(msgDataType msg)
{
    #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
    qDebug("the tcp msg type is %ld\n",msg.msgType);
    qDebug("the tcp msg Id is %d\n",msg.msgObj.msgId);
    qDebug("the tcp msg dataLen is %d\n",msg.msgObj.dataLen);
    qDebug("the tcp msg data is %s\n",msg.msgObj.data);
    #endif
    if(!bWriteData)
        return;
    writeLen = msg.msgObj.dataLen;
    memcpy(writeBuf,msg.msgObj.data,writeLen);
    msgId = msg.msgObj.msgId;
}
/**********************************************************************************/
void tcpSocketThread::disconnectToHost(void)
{
    runFlag = 0;
}
/**********************************************************************************/
void tcpSocketThread::quitPthread(void)
{
    #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
    qDebug()<<"quit socket pthread...................";
    #endif
    emit clientConnectStateChange("");
    emit clientDisconnect(ptr);
}
/**********************************************************************************/
