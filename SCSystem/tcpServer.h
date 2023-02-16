#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <qobject.h>
#include <QTcpServer>

#include "tcpSocketPthread.h"
#include "public.h"

//继承QTCPSERVER以实现多线程TCPscoket的服务器。
class tcpServer : public QTcpServer
{
    Q_OBJECT
public:
    tcpServer();
    ~tcpServer();

    //每当一个新的客户端连接时，通过标识码socketDescriptor，实现与对应的客户端通信
    virtual void incomingConnection(qintptr socketDescriptor);//该虚函数是重点,覆盖已获取多线程
public slots:
    void clientDisconnected(int sockDesc); //当产生客户端断开连接时向上层发送信号
    void onClientActivatState(QString strSockDesc);  //设置客户端发送数据激活状态
    void recvMsgData(msgDataType);//接收外部的数据，将外部数据通过TCP传输给下位机
private:
    QList<qintptr> getConnectListptr();

private slots:

signals:
    void sendMsgData(msgDataType);
    void recvReadData(QString);             //接收来自有TCP的数据信号
    void clientConnectStateChange(QString);//客户端连接信息
    void displayMessage(QString);
    void changeClientState(QList<qintptr>, qintptr, bool);
private:
    QMap<int,tcpSocketThread*> tcpClient;
};
#endif
