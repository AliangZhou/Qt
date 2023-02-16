#include "mainwindow.h"
#include "ui_mainwindow.h"

static int add_in = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pClient = new tcpClient(this);
    m_pTimer = new QTimer();
    m_pTimer->setInterval(2000);
    m_pTimer->start();


    connect(m_pTimer, &QTimer::timeout, this, [=]{
        on_pushButton_sendMessage_clicked();
        add_in++;
    });
    connect(m_pClient, &tcpClient::displayMessage, this,  &MainWindow::onDisplayMessage);
    connect(this, &MainWindow::sendDataToServer, m_pClient, &tcpClient::sendSeverData);
}

MainWindow::~MainWindow()
{
    m_pClient->deleteLater();
    delete ui;
}

void MainWindow::onDisplayMessage(const QString& str)
{
    ui->textBrowser_receivedMessages->append(str);
}

void MainWindow::assemblingDatagram(msgDataType &msgData, unsigned short usAddr, int nValue)
{
    unsigned char buf[COMMAND_MAX_LEN] = {0};
    unsigned char data[COMMAND_MAX_LEN] = {0};
    unsigned char *p = data;
    int dataLen = 0;
    int len = 0;

    INT2CHAR(p,DEFINE_SOH_VALUE);
    p=p+4;
    INT2CHAR(p,usAddr);
    p=p+4;
    INT2CHAR(p,nValue);
    p=p+4;

    dataLen = 12;

    len = protolDataCreate(buf,SET_POINT_POS,BINARY,ARM_TO_PC,0,data,dataLen);

    msgData.msgType = 1;
    msgData.msgObj.msgId=1;
    msgData.msgObj.dataLen = len;
    memcpy(msgData.msgObj.data, buf,len);
}

void MainWindow::on_pushButton_sendMessage_clicked()
{
    msgDataType msgData;
    assemblingDatagram(msgData, add_in, 1);
    emit sendDataToServer(msgData);
}
