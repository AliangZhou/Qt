#-------------------------------------------------
#
# Project created by QtCreator 2023-02-13T17:45:15
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SCSystem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpServer.cpp \
    tcpSocketPthread.cpp \
    datapackprotocol.cpp \
    broadcastServer.cpp

HEADERS  += mainwindow.h \
    tcpServer.h \
    tcpSocketPthread.h \
    typedef.h \
    public.h \
    datapackprotocol.h \
    broadcastServer.h

FORMS    += mainwindow.ui
