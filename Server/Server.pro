QT += core websockets
QT -= gui

CONFIG += c++11

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  src/main.cpp \
            src/ChatServer.cpp

HEADERS +=  src/ChatServer.h
