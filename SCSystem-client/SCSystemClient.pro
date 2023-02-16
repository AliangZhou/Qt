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
    tcpClient.cpp \
    dataPackProtocol.cpp

HEADERS  += mainwindow.h \
    tcpClient.h \
    dataPackProtocol.h \
    public.h \
    typedef.h

FORMS    += mainwindow.ui
