#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QtCore>

class TCPThread : public QThread
{
    Q_OBJECT
public:
    explicit TCPThread(int ID, QObject *parent = 0);

    void run();

    QTimer *timer;


signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void sendTCP();
    void disconnected();

private:
    QTcpSocket *socket;

    int socketDescriptor;
    
};

#endif // TCPTHREAD_H
