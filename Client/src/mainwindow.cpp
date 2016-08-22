#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , connectButton(new QPushButton(this))
        , disconnectButton(new QPushButton(this))
        , sendButton(new QPushButton(this))
        , ipInputLine(new QLineEdit(this))
        , portInputLine(new QLineEdit(this))
        , messageInputLine(new QLineEdit(this))
        , nicknameInputLine(new QLineEdit(this))
        , textEditField(new QTextEdit(this))
        , userEditField(new QTextEdit(this))
{
    resize(415, 340);
    setMaximumSize(415, 340);
    setMinimumSize(415, 340);

    ipInputLine->setGeometry(10, 10, 100, 25);
    ipInputLine->setPlaceholderText("IP address");
    ipInputLine->show();

    portInputLine->setGeometry(120, 10, 40, 25);
    portInputLine->setPlaceholderText("Port");
    portInputLine->show();

    connectButton->setGeometry(170, 10, 50, 25);
    connectButton->setText("Connect");
    connectButton->show();

    disconnectButton->setGeometry(230, 10, 65, 25);
    disconnectButton->setText("Disconnect");
    disconnectButton->setEnabled(false);
    disconnectButton->show();

    nicknameInputLine->setGeometry(305, 10, 100, 25);
    nicknameInputLine->setPlaceholderText("Nickname");
    nicknameInputLine->show();

    textEditField->setGeometry(10, 45, 285, 250);
    textEditField->setReadOnly(true);
    textEditField->show();

    userEditField->setGeometry(305, 45, 100, 250);
    userEditField->setReadOnly(true);
    userEditField->show();

    messageInputLine->setGeometry(10, 305, 330, 25);
    messageInputLine->setPlaceholderText("You message...");
    messageInputLine->setEnabled(false);
    messageInputLine->show();

    sendButton->setGeometry(350, 305, 55, 25);
    sendButton->setText("Send");
    sendButton->setEnabled(false);
    sendButton->show();

    connect(connectButton, SIGNAL(clicked()), this, SLOT(connectButton_click()));
    connect(disconnectButton, SIGNAL(clicked(bool)), this, SLOT(disconnectButton_click()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendButton_click()));
    connect(&myWebSocket, SIGNAL(messageSignal(QString)), this, SLOT(processMessage(QString)));
    connect(&myWebSocket, SIGNAL(connectedSignal()), this, SLOT(processConnection()));
    connect(&myWebSocket, SIGNAL(errorSignal()), this, SLOT(errorConnection()));
}

MainWindow::~MainWindow()
{
    delete connectButton;
    delete disconnectButton;
    delete sendButton;
    delete ipInputLine;
    delete portInputLine;
    delete messageInputLine;
    delete nicknameInputLine;
    delete textEditField;
    delete userEditField;
}

void MainWindow::connectButton_click()
{
    ipInputLine->setEnabled(false);
    portInputLine->setEnabled(false);
    nicknameInputLine->setEnabled(false);

    myWebSocket.connectSocket(QUrl("ws://" + ipInputLine->text() +":" + portInputLine->text()));
}

void MainWindow::disconnectButton_click()
{
    connectButton->setEnabled(true);
    disconnectButton->setEnabled(false);
    sendButton->setEnabled(false);
    ipInputLine->setEnabled(true);
    portInputLine->setEnabled(true);
    nicknameInputLine->setEnabled(true);
    messageInputLine->setEnabled(false);
    textEditField->append("Disconnected");
    userEditField->clear();

    myWebSocket.disconnectSocket();
}

void MainWindow::sendButton_click()
{
    QJsonObject jsonMessage;

    jsonMessage["type"] = MESSAGE;
    jsonMessage["nick"] = nicknameInputLine->text();
    jsonMessage["text"] = messageInputLine->text();

    QJsonDocument doc(jsonMessage);

    myWebSocket.sendMessage(doc.toJson(QJsonDocument::Compact));

    messageInputLine->clear();
}

void MainWindow::processMessage(QString message)
{
    QJsonDocument document = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonMessage = document.object();

    switch (jsonMessage["type"].toInt())
    {

        case MESSAGE:
                {
                    QString parsedMessage = jsonMessage["nick"].toString() + ": " + jsonMessage["text"].toString();

                    textEditField->append(parsedMessage);
                }
            break;

        case INFO:
                {
                    userEditField->clear();

                    QJsonArray userList = jsonMessage["userList"].toArray();

                    for (int i = 0; i < userList.size(); i++)
                        userEditField->append(userList[i].toString());
                }
            break;
        default:
            break;
    }
}

void MainWindow::processConnection()
{
    disconnectButton->setEnabled(true);
    connectButton->setEnabled(false);
    ipInputLine->setEnabled(false);
    portInputLine->setEnabled(false);
    nicknameInputLine->setEnabled(false);
    sendButton->setEnabled(true);
    messageInputLine->setEnabled(true);
    textEditField->clear();
    textEditField->append("Connected");

    sendInfo();
}

void MainWindow::errorConnection()
{
    ipInputLine->setEnabled(true);
    portInputLine->setEnabled(true);
    nicknameInputLine->setEnabled(true);
    disconnectButton->setEnabled(false);
    connectButton->setEnabled(true);

    userEditField->clear();
    textEditField->append("ERROR: Connection with the server is absent");
}

void MainWindow::sendInfo()
{
    QJsonObject jsonMessage;
    jsonMessage["type"] = INFO;
    jsonMessage["nick"] = nicknameInputLine->text();

    QJsonDocument doc(jsonMessage);
    myWebSocket.sendMessage(doc.toJson(QJsonDocument::Compact));
}
