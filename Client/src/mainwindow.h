#pragma once

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>

#include "WebSocketClient.h"

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT

    Q_ENUMS(TypeName)

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    enum TypeName {MESSAGE = 0, INFO};

private:
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QPushButton *sendButton;
    QLineEdit *ipInputLine;
    QLineEdit *portInputLine;
    QLineEdit *messageInputLine;
    QLineEdit *nicknameInputLine;
    QTextEdit *textEditField;
    QTextEdit *userEditField;

    WebSocketClient& myWebSocket = WebSocketClient::getInstance();

    void sendInfo();

private slots:
    void connectButton_click();
    void disconnectButton_click();
    void sendButton_click();
    void processMessage(QString message);
    void processConnection();
    void errorConnection();
};
