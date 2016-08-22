#pragma once

#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QList>
#include <QObject>
#include <QStringList>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

#include <iostream>

struct ClientInfo
{
    ClientInfo(QWebSocket *socket)
        : socket(socket)
    {}

    QWebSocket *socket;
    QString name;
};

class ChatServer : public QObject
{
private:
    Q_OBJECT

    Q_ENUMS(TypeName)

public:
    ChatServer(int port, QObject *parent = Q_NULLPTR);
    virtual ~ChatServer();

    enum TypeName {MESSAGE = 0, INFO};

private slots:
    void newConnection();
    void processMessage(QString message);
    void socketDisconnected();

private:
    QWebSocketServer *serverSocket_;
    QList <ClientInfo*> clientSockets_;

    void sendAllClients(QString textMessage);
    void sendUserList();
};
