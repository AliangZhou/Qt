/*********************************************************************************
*Author:zhouFuLiang
*Date: 2023-02-15
*Description: 服务端UDP广播包，发送数据逻辑
**********************************************************************************/
#ifndef BROADCAST_SERVER_H
#define BROADCAST_SERVER_H

#include <qobject.h>
#include <QUdpSocket>
#include <QTimer>

#include "public.h"


#define CALL_DEVICE_NAME    "SCSystem_01"

class BroadcastServer : public QObject
{
     Q_OBJECT
public:
    BroadcastServer();
    ~BroadcastServer();
private:
    QUdpSocket *udpSocketP;
    QTimer *timerP;
private slots:
    void onTimerOut(void);
    void readyReadSlot(void);
};
#endif
