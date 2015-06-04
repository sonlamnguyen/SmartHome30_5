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

#include "uartthread.h"
#include "mysql.h"
#include "arduPi.h"
#include "SmartBus.h"
#include "tcpthread.h"
#include "smartbusfuntion.h"

SmartBusFuntion readStatus;



UARTThread::UARTThread(QObject *parent) :
    QThread(parent)
{
    readStatus.InitSmartBus(9600);

}

void UARTThread::run()
{
    char SubnetID, RTUID, AreaNo, ChannelNo, Value, SceneNo;
    char ValueChannel[8];

    while(1)
    {
        if(readStatus.getAllowTransmit()==0)
        {

            readStatus.ReadValueRandom();

        }else
            if(readStatus.getAllowTransmit()==1)
            {
                switch(readStatus.getFunctionTrasmit())
                {
                case SCENE_CONTROL:
                {
                    SubnetID = readStatus.getSceneControlValue(0);
                    RTUID    = readStatus.getSceneControlValue(1);
                    AreaNo   = readStatus.getSceneControlValue(2);
                    SceneNo  = readStatus.getSceneControlValue(3);

                    readStatus.SceneControlUART(SubnetID,RTUID,AreaNo,SceneNo);
                    delay(100);
                    readStatus.setAllowTransmit(0);
                    readStatus.setFunctionTrasmit(0);
                    break;
                }
                case SINGLE_CHANNEL_CONTROL:
                {

                    SubnetID = readStatus.getSingleChannelControlValue(0);
                    RTUID    = readStatus.getSingleChannelControlValue(1);
                    ChannelNo= readStatus.getSingleChannelControlValue(2);
                    Value    = readStatus.getSingleChannelControlValue(3);

                    readStatus.SingleChannelControlUART(SubnetID,RTUID,ChannelNo,Value);
                    delay(100);
                    readStatus.setAllowTransmit(0);
                    readStatus.setFunctionTrasmit(0);
                    break;
                }
                case MODIFY_SCENE_CONFIGURATION:
                {
                    SubnetID = readStatus.getModifySceneConfigurationValue(0);
                    RTUID    = readStatus.getModifySceneConfigurationValue(1);
                    AreaNo   = readStatus.getModifySceneConfigurationValue(2);
                    SceneNo  = readStatus.getModifySceneConfigurationValue(3);
                    for(int i=0;i<8;i++)
                        ValueChannel[i]= readStatus.getModifySceneConfigurationValue(i+4);
                    readStatus.ModifySceneConfigurationUART(SubnetID,RTUID,AreaNo,SceneNo,ValueChannel);
                    delay(100);
                    readStatus.setAllowTransmit(0);
                    readStatus.setFunctionTrasmit(0);
                    break;
                }
                case READ_STATUS_OF_CHANNEL:
                {
                    SubnetID = readStatus.getReadStatusOfChannelValue(0);
                    RTUID    = readStatus.getReadStatusOfChannelValue(1);
                    readStatus.ReadStatusOfChannelUART(SubnetID,RTUID);
                    delay(100);
                    readStatus.setAllowTransmit(0);
                    readStatus.setFunctionTrasmit(0);
                    break;
                }
                }

            }

    }
}
