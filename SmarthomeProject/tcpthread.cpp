#include <QDebug>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <QString>
#include <QNetworkInterface>
#include <QTableView>
#include <QSqlTableModel>
#include <QtSql>
#include <QtCore>

#include "mysql.h"
#include "arduPi.h"
#include "SmartBus.h"
#include "tcpthread.h"
#include "smartbusfuntion.h"


QByteArray buffer[1000];
int i=0;

SmartBusFuntion smartbus;

QString SceneControl=  QString("SceneControl");
QString SingleChannelControl=  QString("SingleChannelControl");
QString ModifySceneConfiguration = QString("ModifySceneConfiguration");
QString ReadStatusOfChannel = QString("ReadStatusOfChannel");
QString ModifySubnetIDAndDeviceID = QString("ModifySubnnetIDAndDeviceID");


TCPThread::TCPThread(int ID, QObject *parent) :
    QThread(parent)
{

    this->socketDescriptor = ID;
    smartbus.InitSmartBus(9600);

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()),this,SLOT(sendTCP()));
    timer->start(500);

}
void TCPThread::run()
{
    //thread starts here
    // qDebug() << "Thread started";
    //printf("Thread started\n");

    socket = new QTcpSocket();

    //set the ID
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        //something's wrong, we just emit a signal
        emit error(socket->error());
        return;
    }

    // connect socket and signal
    //note - Qt::DirectConnection is used because it's multithreaded
    //this makes the slot to be invoked immediately, when the signal is emitted

    connect(socket, SIGNAL(readyRead()), this , SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    //we'll have multiple clients, we want to know which is which
    qDebug() << socketDescriptor << "Client connected";


    //make this thread a loop
    //thread will stay alive so that signal/slot to funtion properly
    //not dropped out in the middle when thread dies

    exec();


}

void TCPThread::sendTCP()
{

    switch(smartbus.getNewEvent())
    {
    case 1:
    {
        qDebug()<<smartbus.getNewEvent();
        socket->write("Relay:");
        for(int i=1;i<smartbus.getRelayValueEvent(0);i++)
        {
            std::ostringstream ss1;
            ss1 <<smartbus.getRelayValueEvent(i)<<" ";
            const char *transmitSceneControl1 = ss1.str().c_str();
            socket->write(transmitSceneControl1);
        }
        smartbus.setNewEvent(0);
        break;
    }
    case 2:
    {
        qDebug()<<smartbus.getNewEvent();
        socket->write("Dimmer:");
        for(int i=1;i<smartbus.getRelayValueEvent(0);i++)
        {
            std::ostringstream ss1;
            ss1 <<smartbus.getDimmervalueEvent(i)<<" ";
            const char *transmitSceneControl1 = ss1.str().c_str();
            socket->write(transmitSceneControl1);
        }
        smartbus.setNewEvent(0);
        break;
    }
    }
}
void TCPThread::readyRead()
{

    int Received[40];
    int statusScene[8]={1,2,4,8,16,32,64,128};

    //get the information
    QString bufferRead, result;
    //Read Value from tcp
    bufferRead = socket->readAll();

    result = bufferRead.mid(0,80);
    qDebug()<<result;

    if(result.contains(SceneControl))
    {
        smartbus.SceneControlTCP(result);
        //wait for smartbus transmit finished
        while(1)
        {
            if(smartbus.getFinishReceived()==1)
                break;
        }

        Received[0] = smartbus.getSeneControlResponse(0);
        Received[12] = smartbus.getSeneControlResponse(12);
        qDebug("%X",Received[12]); //status

        if(Received[0]>0)
        {
            socket->write("SceneStatus ");
            for(i =0;i<8;i++)
            {
                if((Received[12]&statusScene[i])==statusScene[i])
                {
                    socket->write("100");
                }else
                {
                    socket->write("0");
                }
                socket->write(" ");
            }
        }else
        {
            socket->write("-1");
        }
        smartbus.setFinishReceived(0);
        smartbus.resetBufferReceived();


    }else
        if(result.contains(SingleChannelControl))
        {
            smartbus.SingleChannelControlTCP(result);
            //wait for smartbus transmit finished
            while(1)
            {
                if(smartbus.getFinishReceived()==1)
                    break;
            }
            Received[0]= smartbus.getSingleChanneControlResponse(0);
            //Received data

            Received[11] = smartbus.getSingleChanneControlResponse(11);

            if(Received[0]>0)
            {
                std::ostringstream ss1;
                ss1 <<Received[11];
                const char *transmitSingleChannelControl = ss1.str().c_str();
                socket->write(transmitSingleChannelControl);

            }else
            {
                socket->write("-1");
            }
            smartbus.setFinishReceived(0);
            smartbus.resetBufferReceived();


        }else
            if(result.contains(ModifySceneConfiguration))
            {
                smartbus.ModifySceneConfigurationTCP(result);
                //wait for smartbus transmit finished
                while(1)
                {
                    if(smartbus.getFinishReceived()==1)
                        break;
                }

                //Received data
                Received[0] = smartbus.getModifySceneConfigurationResponse(0);
                Received[9] = smartbus.getModifySceneConfigurationResponse(9);


                if(Received[9]==0xF8 && Received[0]>0)
                {

                    socket->write("1");
                }else
                {
                    socket->write("-1");
                }
                smartbus.setFinishReceived(0);
                smartbus.resetBufferReceived();


            }else
                if(result.contains(ReadStatusOfChannel))
                {
                    smartbus.ReadStatusOfChannelTCP(result);
                    //wait for smartbus transmit finished
                    while(1)
                    {
                        if(smartbus.getFinishReceived()==1)
                            break;
                    }
                    Received[0]=-1;
                    for(i=0;i<8;i++)
                        Received[i+10]=0;

                    Received[0] = smartbus.getReadStatusOfChannelResponse(0);
                    //Received data
                    for(int i=1; i<Received[0];i++)
                    {
                        Received[i] = smartbus.getReadStatusOfChannelResponse(i);

                    }

                    if(Received[0]>0)
                    {
                        socket->write("StatusOfChannel ");
                        for(i=0;i<8;i++)
                        {
                            //Received[10+i] = i;
                            std::ostringstream ss1;
                            ss1 <<Received[i+10]<<" ";
                            const char *transmitSceneControl1 = ss1.str().c_str();
                            socket->write(transmitSceneControl1);
                        }
                    }else
                    {
                        socket->write("-1");
                    }


                    smartbus.setFinishReceived(0);
                    smartbus.resetBufferReceived();
                }


}

void TCPThread::disconnected()
{

    qDebug("%d Disconnected", socketDescriptor);

    socket->deleteLater();
    exit(0);
}

