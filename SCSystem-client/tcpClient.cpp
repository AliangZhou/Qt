/**********************************************************************************/
#include "tcpClient.h"
/**********************************************************************************/
tcpClient::tcpClient(QObject *parent) : QObject(parent)
{
    m_tcpSocket = new QTcpSocket(this);
    udpSocketP = new QUdpSocket(this);

    udpSocketP->bind(BROADCAST_SERVER_PORT,QUdpSocket::ShareAddress);
    connect(udpSocketP,&QUdpSocket::readyRead,this, &tcpClient::readBroadcastData);

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &tcpClient::readSeverMessage);
    QObject::connect(m_tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

    clientConnectFlag = 0;
    readBufIndex = 0;


}
/**********************************************************************************/
tcpClient::~tcpClient()
{

}

void tcpClient::sendSeverData(msgDataType msg)
{
    recvUartData(msg);
}
/**********************************************************************************/
void tcpClient::readBroadcastData(void)
{
	QHostAddress sender;
	quint16 senderPort; 
    if(udpSocketP->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocketP->pendingDatagramSize());
        udpSocketP->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

		quint32 addr_origin = sender.toIPv4Address();
    	QHostAddress addr_host = QHostAddress(addr_origin);
	
		QString readData = QString(datagram);
		if(!clientConnectFlag)//如果客户端没有连接则连接客户端
		{
			if(readData ==QString::fromLocal8Bit(CALL_DEVICE_NAME))//先确认为服务器发过来的广播包
			{
				serverIP = addr_host.toString();
				#ifdef DEBUG_COMMON_INFORMATION_DISPLAY
				qDebug()<<"server IP:" << serverIP;
				#endif
				newConnect();
                emit displayMessage(QString("Connected server ip is: %1").arg(serverIP));
			}
		}
    }
}
/**********************************************************************************/
void tcpClient::newConnect()
{
    m_tcpSocket->abort();//取消已有的连接
    m_tcpSocket->connectToHost(QHostAddress(serverIP),TCP_SERVER_PORT);
    m_tcpSocket->waitForConnected();
    clientConnectFlag = 1;
	#ifdef DEBUG_OK_INFORMATION_DISPLAY
	qDebug() << "connect server ok!!!";
	#endif
}

/**********************************************************************************/
void tcpClient::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<m_tcpSocket->errorString();//输出错误信息
    emit displayMessage(QString("%1").arg(m_tcpSocket->errorString()));
	clientConnectFlag = 0;
}
/**********************************************************************************/
void tcpClient::onTimerOut()
{
	//if(!socketFlag)return;
	
	static int flag = 0;
	
	char data[COMMAND_MAX_LEN]={0};
	char buf[COMMAND_MAX_LEN]={0};
	int dataLen = 0;
	if(flag)
	{
		QString str =
		"sendCount to server is dafasdfasdfsadfasfdsdafasdfsadfasdfasdfsdafsdafsdafsdafsdafsadfsafdsdafsadfsdafsdafsadfasdfasdffffffffffffffffffffffffffffffffffffffffffffffffasdfffffffffffffffffffffffffffffffffffffffffffffasdffffffffffffffffffffffffffffffffffffffffffffasdfffffffffffffsassssssssssssssssssssssssssdsdsds";
		//str.append(QString::number(sendCount));
		strcpy(data,str.toStdString().data());
		//dataLen = protolDataCreate((UCHAR*)buf,DISPLAY_STRING,ASCII,ARM_TO_PC,sendCount,(UCHAR*)data,strlen(data));
		flag = 0;
	}
	else
	{
		QString str = 
		"jeomy hello nihao sendCount to server is .dafdsfasdfasdf.asdfsdafsdaf.asdfsdafsdafmmmmmmmmmmmmmmmmmmmmmmmmmmmmdddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
		//str.append(QString::number(sendCount));
		strcpy(data,str.toStdString().data());
		//dataLen = protolDataCreate((UCHAR*)buf,DISPLAY_STRING,BINARY,ARM_TO_PC,sendCount,(UCHAR*)data,strlen(data));
		flag = 1;
	}
    m_tcpSocket->write(buf,dataLen);
    m_tcpSocket->waitForBytesWritten();
    m_tcpSocket->flush();
	//m_Timer->start(10);//每隔一秒中发送一条数据
	//sendCount++;
}
/**********************************************************************************/
void tcpClient::recvUartData(msgDataType msgData)
{
    #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
    printf("tcp upload data len is %d\n",msgData.msgObj.dataLen);
    #endif
    m_tcpSocket->write(msgData.msgObj.data,msgData.msgObj.dataLen);
    m_tcpSocket->waitForBytesWritten();
    m_tcpSocket->flush();
}

void tcpClient::readSeverMessage()
{
    QString str;
    QVariant frameData;
    unsigned int ret = 0,frameLen=0,bufDatalen=0;
    unsigned char buf[TCP_DATA_MAX_LEN];
    dataFrameContent Obj;
    memset(&Obj, 0, sizeof(dataFrameContent));
    ret = m_tcpSocket->read((char*)buf,TCP_DATA_MAX_LEN);
    if((ret > 0)&&(ret < TCP_DATA_MAX_LEN))//读取到有效的数据
    {
        #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
        qDebug() << "read Data from server len" << ret;
        for(unsigned int i = 0; i < ret; i++)
        {
            printf("%02x ",buf[i]);
        }
        printf("\n");
        #endif
        memcpy(&readBuf[readBufIndex],buf,ret);
        readBufIndex = readBufIndex + ret;
        bufDatalen=readBufIndex;
        while(1)
        {
            memset(buf,0,sizeof(buf));
            frameLen=0;
            ret = recvDataParser(readBuf,(unsigned char*)buf,&frameLen,&bufDatalen);
            #ifdef DEBUG_KEY_INFORMATION_DISPLAY
            qDebug()<<"the ret is....."<< ret << "the frameLen..." << frameLen;
            #endif
            if(RETURN_FRAME_OK == ret)//得到一个完整的命令帧
            {
                if(frameLen >= DATA_PACK_FIX_LEN)//完整的数据包
                {
                    protolDataParse((unsigned char*)buf,frameLen,&Obj);
                    frameData.setValue(Obj);
                    if(ASCII==Obj.dataType)
                    {
                        #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
                        printf("the recv data is %s", (char*)Obj.data);
                        #endif
                        emit sendTcpData(frameData);
                    }
                    else
                    {
                        #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
                        printf("the Obj.datalen is ...%d\n", Obj.datalen);
                        for(int i = 0 ; i < Obj.datalen; i++)
                        {
                            printf("%02x ", Obj.data[i]);
                        }
                        printf("\n");
                        #endif
                        emit sendTcpData(frameData);
                    }
                }
            }
            else if(RETURN_FRAME_ERROR == ret)
            {
                break;
            }
            else if(RETURN_CONTINUE_RECV == ret)
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
}

/**********************************************************************************/

