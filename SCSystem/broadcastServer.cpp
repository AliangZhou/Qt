/**********************************************************************************/
#include "broadcastServer.h"
/**********************************************************************************/
BroadcastServer::BroadcastServer() : QObject()
{
    udpSocketP = new QUdpSocket();

    #if 0 //不进行数据读
    udpSocketP->bind(8193,QUdpSocket::ShareAddress);
    QObject::connect(udpSocketP,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
    #endif

    timerP = new QTimer(this);
    timerP->setInterval(1000);//1秒定时器
    timerP->start();
    QObject::connect(timerP,SIGNAL(timeout()),this,SLOT(onTimerOut()));//绑定定时器的信号与槽
}
/**********************************************************************************/
BroadcastServer::~BroadcastServer()
{

}
/**********************************************************************************/
void BroadcastServer::onTimerOut()
{
    //往socket端口写数据,向网络中发送广播包
    udpSocketP->writeDatagram(CALL_DEVICE_NAME,QHostAddress::Broadcast,BROADCAST_SERVER_PORT);
}
/**********************************************************************************/
void BroadcastServer::readyReadSlot(void)
{
    qDebug()<<"read broadcast from server......";
    if(udpSocketP->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocketP->pendingDatagramSize());
        udpSocketP->readDatagram(datagram.data(),datagram.size());
        qDebug()<<"read data is" << datagram;
    }
}
/**********************************************************************************/
