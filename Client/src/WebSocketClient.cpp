#include "WebSocketClient.h"

WebSocketClient::WebSocketClient(QObject *parent)
        : QObject(parent)
        , socket_(new QWebSocket)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError") ;

    connect(socket_, SIGNAL(textMessageReceived(QString)), this, SIGNAL(messageSignal(QString)));
    connect(socket_, SIGNAL(connected()), this, SIGNAL(connectedSignal()));
    connect(socket_, SIGNAL(connected()), this, SLOT(setQtConnect()));
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(errorSignal()));
}

WebSocketClient::~WebSocketClient()
{
    delete socket_;
}

void WebSocketClient::connectSocket(QUrl _urlAddress)
{
    socket_->open(_urlAddress);
}

void WebSocketClient::disconnectSocket()
{
    socket_->close();
    disconnect(socket_, SIGNAL(textMessageReceived(QString)), this, SLOT(processMessage(QString)));
}

void WebSocketClient::sendMessage(QString message)
{
    socket_->sendTextMessage(message);
}

void WebSocketClient::setQtConnect()
{
    connect(socket_, SIGNAL(disconnected()), this, SLOT(disconnectSocket()));
}
