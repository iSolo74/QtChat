QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Client
TEMPLATE = app

SOURCES +=  src/main.cpp\
            src/WebSocketClient.cpp \
            src/MainWindow.cpp

HEADERS +=  src/WebSocketClient.h \
            src/MainWindow.h

