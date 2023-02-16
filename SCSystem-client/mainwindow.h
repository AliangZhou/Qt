#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QHostAddress>
#include <QMessageBox>
#include "tcpClient.h"
#include <QTimer>


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
    void sendDataToServer(msgDataType);
private slots:
    void onDisplayMessage(const QString& str);
    void on_pushButton_sendMessage_clicked();

private:
    void assemblingDatagram(msgDataType& msgData, unsigned short usAddr, int nValue);

private:
    Ui::MainWindow *ui;
    tcpClient* m_pClient;
    QTimer *m_pTimer;
};

#endif // MAINWINDOW_H
