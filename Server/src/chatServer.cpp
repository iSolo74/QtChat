#include "chatserver.h"

using namespace std;

ChatServer::ChatServer(int port, QObject *parent)
        : QObject(parent)
        , serverSocket_(new QWebSocketServer("Chat Server", QWebSocketServer::NonSecureMode, this))
{
    if (!serverSocket_->listen(QHostAddress::Any, quint16(port)))
    {
        cout << "ERROR: Can't open port " << port << endl;

        while (!serverSocket_->listen(QHostAddress::Any, quint16(port)))
            port++;
    }

    connect(serverSocket_, SIGNAL(newConnection()), this, SLOT(newConnection()));

    cout << "Server listening on port " << port << endl;
}

ChatServer::~ChatServer()
{
    serverSocket_->close();
    qDeleteAll(clientSockets_.begin(), clientSockets_.end());
}

void ChatServer::newConnection()
{
    QWebSocket *newClientSocket = serverSocket_->nextPendingConnection();

    connect(newClientSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(processMessage(QString)));
    connect(newClientSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    ClientInfo *clientInfo = new ClientInfo(newClientSocket);

    clientSockets_.append(clientInfo);

    cout << "New client" << endl;
}

void ChatServer::processMessage(QString message)
{
    QJsonDocument document = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonMessage = document.object();

    switch (jsonMessage["type"].toInt())
    {
        case MESSAGE:
                {
                    sendAllClients(message);
                }
            break;

        case INFO:
                {
                    QJsonArray clientList;
                    QJsonObject reply;

                    QWebSocket *pClient = qobject_cast<QWebSocket*>(sender());

                    for (int i = 0; i < clientSockets_.size(); i++)
                    {
                        if (clientSockets_.at(i)->socket == pClient)
                            clientSockets_.at(i)->name = jsonMessage["nick"].toString();

                        clientList.append(clientSockets_.at(i)->name);
                    }

                    reply["type"] = INFO;
                    reply["userList"] = clientList;

                    QJsonDocument doc(reply);

                    sendAllClients(doc.toJson(QJsonDocument::Compact));
                }
            break;
        default:
            break;
    }
}

void ChatServer::socketDisconnected()
{
    cout << "Client disconnected" << endl;
    QWebSocket *pClient = qobject_cast<QWebSocket*>(sender());

    for (int i = 0; i < clientSockets_.size(); ++i)
    {
        if (clientSockets_.at(i)->socket == pClient)
        {
            clientSockets_.removeAt(i);
            pClient->deleteLater();

            break;
        }
    }

    sendUserList();
}

void ChatServer::sendAllClients(QString textMessage)
{
    Q_FOREACH (ClientInfo *pClient, clientSockets_)
    {
        pClient->socket->sendTextMessage(textMessage);
    }
}

void ChatServer::sendUserList()
{
    QJsonArray clientList;
    QJsonObject reply;

    for (int i = 0; i < clientSockets_.size(); ++i)
    {
        clientList.append(clientSockets_.at(i)->name);
    }

    reply["type"] = INFO;
    reply["userList"] = clientList;

    QJsonDocument document(reply);

    sendAllClients(document.toJson(QJsonDocument::Compact));
}
