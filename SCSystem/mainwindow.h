#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "tcpServer.h"
#include "broadcastServer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void clientActivatState(QString);
    void sendData(msgDataType);//发送TCP数据给client
private:
    void connectPart();
    void setRegisterValue(msgDataType& msgData, unsigned short usAddr, int nValue);
private slots:
    void onDisplayMessage(const QString& str);
    void on_pushButton_sendMessage_clicked();
    void onChangeClientState(QList<qintptr> listptr, qintptr mptr, bool bState);
private:
    Ui::MainWindow *ui;
    tcpServer* m_server;
    BroadcastServer *m_broadSer;
};

#endif // MAINWINDOW_H
