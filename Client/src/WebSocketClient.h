#pragma once

#include <QAbstractSocket>
#include <QObject>
#include <QString>
#include <QWebSocket>

class WebSocketClient : public QObject
{
private:
    Q_OBJECT

public:
    static WebSocketClient& getInstance()
    {
        static WebSocketClient s;
        return s;
    }

    void connectSocket(QUrl _urlAddress);
    void disconnectSocket();
    void sendMessage(QString message);

private:
    WebSocketClient(QObject *parent = Q_NULLPTR);
    virtual ~WebSocketClient();

    WebSocketClient(WebSocketClient const&) = delete;
    WebSocketClient& operator= (WebSocketClient const&) = delete;

    QWebSocket *socket_;

private slots:
    void setQtConnect();

signals:
    void messageSignal(QString message);
    void connectedSignal();
    void errorSignal();
};
