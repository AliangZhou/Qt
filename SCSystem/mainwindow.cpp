#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("SCSystem Sever"));
    m_server = new tcpServer();
    m_broadSer = new BroadcastServer();
    connectPart();
}

MainWindow::~MainWindow()
{
    m_server->close();
    m_server->deleteLater();

    delete ui;
}

void MainWindow::connectPart()
{
    connect(m_server, &tcpServer::displayMessage, this, &MainWindow::onDisplayMessage);
    connect(m_server, &tcpServer::changeClientState, this, &MainWindow::onChangeClientState);
    connect(ui->comboBox_receiver, &QComboBox::currentTextChanged, m_server, &tcpServer::onClientActivatState);
    connect(this, &MainWindow::sendData, m_server, &tcpServer::recvMsgData);
}

void MainWindow::setRegisterValue(msgDataType &msgData, unsigned short usAddr, int nValue)
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

    len = protolDataCreate(buf,SET_POINT_POS,BINARY,PC_TO_ARM,0,data,dataLen);

    msgData.msgType = 1;
    msgData.msgObj.msgId=1;
    msgData.msgObj.dataLen = len;
    memcpy(msgData.msgObj.data, buf,len);
}

void MainWindow::on_pushButton_sendMessage_clicked()
{
    msgDataType msgData;
    setRegisterValue(msgData, 1, 1);
    emit sendData(msgData);
}


void MainWindow::onDisplayMessage(const QString& str)
{
    ui->textBrowser_receivedMessages->append(str);
}

void MainWindow::onChangeClientState(QList<qintptr> listptr, qintptr mptr, bool bState)
{
    ui->comboBox_receiver->clear();
    ui->comboBox_receiver->addItem("Broadcast");
    foreach (qintptr ptr, listptr) {
        ui->comboBox_receiver->addItem(QString::number(ptr));
    }
    if(bState){
        onDisplayMessage(QString("A client enter the room, ID: %1").arg(mptr));
    }else{
        onDisplayMessage(QString("A client leave the room, ID: %1").arg(mptr));
    }

}
