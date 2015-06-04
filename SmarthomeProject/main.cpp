#include <QApplication>
#include "tcpthread.h"
#include "uartthread.h"
#include "smartbusfuntion.h"
#include "mysql.h"
#include <stdlib.h>
#include "myserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mysql mysql1;
    mysql1.MakeConnectMySQL("localhost","root","smarthome","smarthome");
    qDebug()<<mysql1.getAddressMac();

    MyServer server;
    server.startServer();

    UARTThread uartThread;
    uartThread.start();

    return a.exec();
}
