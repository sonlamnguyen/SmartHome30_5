#include "myserver.h"
#include <QDebug>
#include <QTcpSocket>
#include <stdio.h>
#include "tcpthread.h"

MyServer::MyServer(QObject *parent) :
    QTcpServer(parent)
{
}

void MyServer::startServer()
{
    int port=1234;

    if(!this->listen(QHostAddress::Any,port))
    {
        qDebug("Could not start server");
    }
    else
    {
        qDebug() << "Listening to port " << port << " ... ";
    }

}

//this function is called by QTcpServer when a new connection is available.
void MyServer::incomingConnection(int socketDescriptor)
{
    //socketDescriptor = 13;
    //We have a new connection
    qDebug() << socketDescriptor << "Connecting ...";

    //Every new connection will be run in a newly created thread
    TCPThread *thread = new TCPThread(socketDescriptor, this);

    //connect signal/slot
    //once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();

}
