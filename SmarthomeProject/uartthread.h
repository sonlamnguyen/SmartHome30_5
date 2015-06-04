#ifndef UARTTHREAD_H
#define UARTTHREAD_H

#include <QThread>
#include <arduPi.h>

class UARTThread : public QThread
{
    Q_OBJECT
public:
    explicit UARTThread(QObject *parent = 0);
    void run();


signals:

public slots:
};

#endif // UARTTHREAD_H
