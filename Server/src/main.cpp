#include <QCoreApplication>

#include "ChatServer.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "ERROR: Port absent" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);

    QCoreApplication a(argc, argv);

    ChatServer server(port);

    return a.exec();
}
