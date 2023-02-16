#ifndef TCP_SOCKET_PTHREAD_H
#define TCP_SOCKET_PTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QByteArray>
#include <QTimer>
#include <QHBoxLayout>
#include <QHostAddress>

#include "public.h"
#include "datapackprotocol.h"

class tcpSocketThread : public QThread
{
    Q_OBJECT
public:
    tcpSocketThread();
    ~tcpSocketThread();
    void write_ptr(qintptr p){ ptr=p; }
    void setWriteDataState(bool bWrite);
private://定义自己需要的方法或变量
    qintptr ptr;
    QTcpSocket *socket;//客户端的定义
    int msgId;
    unsigned char readBuf[TCP_DATA_MAX_LEN];//从socket客户端读到的数据存放缓冲区
    unsigned int readBufIndex;//缓冲区中空闲空间的当前索引
    unsigned char writeBuf[TCP_DATA_MAX_LEN];
    unsigned int writeLen;
    int runFlag;
    bool bWriteData;    //是否写数据
private slots:
    void sockRecvData(void);
    void recvTcpSendData(msgDataType);//接受外部要通过TCP传输的数据
    void disconnectToHost(void);//客户端断开槽函数
    void quitPthread(void);
signals:
    void sendReadData(QString);//发送从TCP读取的数据
    void clientDisconnect(int sockDesc);
    void clientConnectStateChange(QString);
protected:
    virtual void run();
};
#endif
