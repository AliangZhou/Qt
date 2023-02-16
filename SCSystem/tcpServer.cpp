/**********************************************************************************/
#include "tcpServer.h"
/**********************************************************************************/
tcpServer::tcpServer()
{
    if(!this->listen(QHostAddress::AnyIPv4, TCP_SERVER_PORT))//监听服务器IP和端口
    {
        qDebug() << "SCSytem Server listen error: " << this->errorString();
    }
    else
    {
        qDebug() << "SCSytem Server listen succeed!!!";
    }
}
/**********************************************************************************/
tcpServer::~tcpServer()
{
    foreach (tcpSocketThread* socket, tcpClient)
    {
        socket->quit();
        socket->deleteLater();
    }
}
/**********************************************************************************/
void tcpServer::incomingConnection(qintptr socketDescriptor)
{
    //有新的客户端连接
    //每个客户端连接的tcpSocket分别分配一个专门的线程来处理
    //继承QTcpServer为每个客户端连接时分配线程，并接受处理tcpScoket的信号和槽，还有发送信息，储存连接信息等
    #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
    qDebug()<<"New Connect is connect "<<socketDescriptor;
    #endif
    tcpSocketThread *socketThreadP = NULL;
    socketThreadP = new tcpSocketThread(); //创建服务器处理进程
    socketThreadP->write_ptr(socketDescriptor);
    socketThreadP->start();
    QObject::connect(this, SIGNAL(sendMsgData(msgDataType)),socketThreadP,SLOT(recvTcpSendData(msgDataType)));
    QObject::connect(socketThreadP,SIGNAL(sendReadData(QString)),this, SIGNAL(recvReadData(QString)));
    QObject::connect(socketThreadP,SIGNAL(clientConnectStateChange(QString)),this, SIGNAL(clientConnectStateChange(QString)));
    //服务器进程发出消息，告诉服务器，XX客户端断开连接
    QObject::connect(socketThreadP, SIGNAL(clientDisconnect(int)), this, SLOT(clientDisconnected(int)));
    tcpClient.insert(socketDescriptor,socketThreadP);//插入到连接信息中
    emit changeClientState(getConnectListptr(),socketDescriptor,true);
}
/**********************************************************************************/
void tcpServer::recvMsgData(msgDataType msg)
{
    emit sendMsgData(msg);
}
/**********************************************************************************/
//这是一个槽函数,用于在界面上显示服务器断开了
void tcpServer::clientDisconnected(int sockDesc)
{
    #ifdef DEBUG_COMMON_INFORMATION_DISPLAY
    qDebug() << "client disconnect......"<<sockDesc;
    #endif
    tcpSocketThread *client = tcpClient.value(sockDesc);
    tcpClient.remove(sockDesc);//连接管理中移除断开连接的socket
    delete client;//释放断开连接的资源，子对象线程也会释放
    emit changeClientState(getConnectListptr(), sockDesc, false);
}

void tcpServer::onClientActivatState(QString strSockDesc)
{
//    qDebug() << "set client activat state: " << strSockDesc;
    QMapIterator<int,tcpSocketThread*> iter(tcpClient);
    while (iter.hasNext()) {
        iter.next();
        if(strSockDesc == "Broadcast"){
            iter.value()->setWriteDataState(true);
        }else{
            if(iter.key() == strSockDesc.toInt()){
                iter.value()->setWriteDataState(true);
            }else{
                iter.value()->setWriteDataState(false);
            }
        }
    }
}

QList<qintptr> tcpServer::getConnectListptr()
{
    QList<qintptr> tmp;
    QMapIterator<int,tcpSocketThread*> iter(tcpClient);
    while (iter.hasNext()) {
        iter.next();
        tmp.append(iter.key());
    }
    return tmp;
}
/**********************************************************************************/
