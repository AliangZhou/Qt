#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QHostAddress>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "public.h"
#include "dataPackProtocol.h"

#define CALL_DEVICE_NAME    "SCSystem_01"

/**********************************************************************************/
class tcpClient : public QObject
{
	Q_OBJECT
public:
    tcpClient(QObject *parent=Q_NULLPTR);
	~tcpClient();

signals:
    void sendTcpData(QVariant frameData);
    void displayMessage(QString);
public slots:
    void sendSeverData(msgDataType msg);

private slots:
    void readBroadcastData(void);
	void newConnect();//连接服务器
	void displayError(QAbstractSocket::SocketError);  //显示错误
	void onTimerOut();
	void recvUartData(msgDataType);
    void readSeverMessage();

private:
    QUdpSocket *udpSocketP;//接收广播包的数据
    QTcpSocket *m_tcpSocket;
    QString serverIP;
    int clientConnectFlag;
    unsigned char readBuf[TCP_DATA_MAX_LEN];//从socket客户端读到的数据存放缓冲区
    unsigned int readBufIndex;//缓冲区中空闲空间的当前索引
};
/**********************************************************************************/
#endif

