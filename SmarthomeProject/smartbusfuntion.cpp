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
#include <QTcpSocket>
#include <QtCore>

#include "tcpthread.h"
#include "mysql.h"
#include "arduPi.h"
#include "SmartBus.h"
#include "smartbusfuntion.h"

char  allowTransmit=0;
char  functionTransmit;
char  finishReceived;

char channelStatus[10]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

char Recieved[100];
char RecievedButton[100];
char RecievedRelay[100];
char RecievedDimmer[100];
char SubnetIDButton, RTUIDButton, StatusButton;
char SubnetIDRelay, RTUIDRelay, StatusRelay;
char SubnetIDDimmer, RTUIDDimmer, StatusDimmer;

char SceneControlValue[10];
char SceneControlResponse[40];

char statusSceneControl;

char SingleChannelControlValue[10];
char SingleChannelControlResponse[40];

char ModifySceneConfigurationValue[30];
char ModifySceneConfigurationResponse[40];

char ReadStatusOfChannelValue[10];
char ReadStatusOfChannelResponse[40];

SmartBusRas smartbusF;
Mysql mysql;

SmartBusFuntion::SmartBusFuntion()
{

}

void SmartBusFuntion::InitSmartBus(int baund)
{
    allowTransmit = 0;
    finishReceived= 0;
    smartbusF.SmartBus_init(baund);
    smartbusF.setOriginalID(OriginalSubnetID,OriginalDeviceID,OriginaldeviceType,QTYOfChannel);
}

void SmartBusFuntion::setAllowTransmit(char value)
{
    allowTransmit=value;

}
char SmartBusFuntion::getAllowTransmit(void)
{
    return allowTransmit;
}

void SmartBusFuntion::setFunctionTrasmit(char index)
{
    functionTransmit = index;
}
char SmartBusFuntion::getFunctionTrasmit(void)
{
    return functionTransmit;
}

void SmartBusFuntion::setFinishReceived(char Value)
{
    finishReceived = Value;
}
char SmartBusFuntion::getFinishReceived(void)
{
    return finishReceived;
}

////////////////////////////////////////////////////////
//Scene Control

void SmartBusFuntion::SceneControlTCP(QString resultFromTCP)
{
    allowTransmit=1;
    functionTransmit=SCENE_CONTROL;

    QString result1 , result2 , result3;
    char SubnetID, RTUID, AreaNo, SceneNo;
    int space;
    int i,j;
    QString Space=" ";
    //Find Subnet ID
    result1 = resultFromTCP.mid(13,20);
    for(i=0;i<20;i++)
    {
        if(result1.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    SubnetID = findValue(result1.mid(0,space),0);
    SceneControlValue[0]=SubnetID;
    //Find RTUID
    result2 = result1.mid(space+1,20);
    for(i=0;i<20;i++)
    {
        if(result2.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    RTUID  = findValue(result2.mid(0,space),0);
    SceneControlValue[1]=RTUID;
    //Find Area No
    result3 = result2.mid(space+1,20);
    for(i=0;i<20;i++)
    {
        if(result3.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    AreaNo  = findValue(result3.mid(0,space),0);
    SceneControlValue[2] = AreaNo;
    //Find Scene no
    SceneNo = findValue(result3.mid(space+1,20),0);
    SceneControlValue[3] = SceneNo;

}
void SmartBusFuntion::SceneControlUART(char SubnetID, char RTUID,char AreaNo,char SceneNo)
{

    smartbusF.sceneControl(SubnetID,RTUID,AreaNo,SceneNo);

    int j;
    for(j=0;j<smartbusF.lenghtData();j++)
    {
        SceneControlResponse[j]=smartbusF.ReceivedData(j);
        //qDebug("%X",SceneControlResponse[j]);
        delay(1);
    }

    finishReceived = 1;


}
char SmartBusFuntion::getSeneControlResponse(int index)
{

    return SceneControlResponse[index];
}
char SmartBusFuntion::getSceneControlValue(int index)
{
    return SceneControlValue[index];
}

////////////////////////////////////////////////////////
//Single channel Control

void SmartBusFuntion::SingleChannelControlTCP(QString resultFromTCP)
{
    allowTransmit=1;
    functionTransmit=SINGLE_CHANNEL_CONTROL;


    QString result1 , result2 , result3;
    char SubnetID, RTUID, ChannelNo,Value;
    int space;
    int i;
    QString Space=" ";

    //Find Subnet ID
    result1 = resultFromTCP.mid(21,20);
    for(i=0;i<20;i++)
    {
        if(result1.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }

    SubnetID = findValue(result1.mid(0,space),0);
    SingleChannelControlValue[0] = SubnetID;
    //Find RTUID
    result2 = result1.mid(space+1,20);
    for(i=0;i<20;i++)
    {
        if(result2.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    RTUID  = findValue(result2.mid(0,space),0);
    SingleChannelControlValue[1]=RTUID;
    //Find Channel No
    result3 = result2.mid(space+1,20);
    for(i=0;i<20;i++)
    {
        if(result3.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    ChannelNo  = findValue(result3.mid(0,space),0);
    SingleChannelControlValue[2]= ChannelNo;
    //Find Scene no
    Value = findValue(result3.mid(space+1,20),0);
    SingleChannelControlValue[3]= Value;

}
void SmartBusFuntion::SingleChannelControlUART(char SubnetID,char RTUID, char ChannelNo, char Value)
{

    smartbusF.singleChannelControl(SubnetID,RTUID,ChannelNo,Value,0x00);


    int j;
    for(j=0;j<smartbusF.lenghtData();j++)
    {
        SingleChannelControlResponse[j]=smartbusF.ReceivedData(j);
        qDebug("%X",SingleChannelControlResponse[j]);
    }
    finishReceived = 1;

}
char SmartBusFuntion::getSingleChanneControlResponse(int index)
{

    return SingleChannelControlResponse[index];
}
char SmartBusFuntion::getSingleChannelControlValue(int index)
{

    return SingleChannelControlValue[index];
}


//modify scene configuration
void SmartBusFuntion::ModifySceneConfigurationTCP(QString resultFromTCP)
{
    allowTransmit=1;
    functionTransmit=MODIFY_SCENE_CONFIGURATION;


    QString result1 , result2 , result3, result4, result5, result6, result7, result8, result9, result10, result11;
    char SubnetID, RTUID, AreaNo, SceneNo;
    char ValueChannel;
    int space;
    int i;
    QString Space=" ";

    //Find Subnet ID
    result1 = resultFromTCP.mid(25,40);
    for(i=0;i<40;i++)
    {
        if(result1.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }

    SubnetID = findValue(result1.mid(0,space),0);
    ModifySceneConfigurationValue[0] = SubnetID;
    //Find RTUID
    result2 = result1.mid(space+1,40);
    for(i=0;i<40;i++)
    {
        if(result2.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    RTUID  = findValue(result2.mid(0,space),0);
    ModifySceneConfigurationValue[1]=RTUID;
    //Find AreaNo
    result3 = result2.mid(space+1,40);
    for(i=0;i<40;i++)
    {
        if(result3.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    AreaNo  = findValue(result3.mid(0,space),0);
    ModifySceneConfigurationValue[2]= AreaNo;
    //Find SceneNo
    result4 = result3.mid(space+1,40);
    for(i=0;i<40;i++)
    {
        if(result4.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    SceneNo  = findValue(result4.mid(0,space),0);
    ModifySceneConfigurationValue[3]= SceneNo;
    //Find value Channel1
    result5 = result4.mid(space+1,40);
    for(i=0;i<40;i++)
    {
        if(result5.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    ValueChannel  = findValue(result5.mid(0,space),0);
    ModifySceneConfigurationValue[4]= ValueChannel;
    //Find value Channel 2
    result6 = result5.mid(space+1,40);
    for(i=0;i<40;i++)
    {
        if(result6.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    ValueChannel  = findValue(result6.mid(0,space),0);
    ModifySceneConfigurationValue[5]= ValueChannel;
    //Find value Channel 3
    result7 = result6.mid(space+1,40);
    for(i=0;i<40;i++)
    {
        if(result7.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    ValueChannel  = findValue(result7.mid(0,space),0);
    ModifySceneConfigurationValue[6]= ValueChannel;
    //Find value Channel 4
    result8 = result7.mid(space+1,40);
    for(i=0;i<40;i++)
    {
        if(result8.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    ValueChannel  = findValue(result8.mid(0,space),0);
    ModifySceneConfigurationValue[7]= ValueChannel;
    //Find value Channel 5
    result9 = result8.mid(space+1,40);
    for(i=0;i<40;i++)
    {
        if(result9.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    ValueChannel  = findValue(result9.mid(0,space),0);
    ModifySceneConfigurationValue[8]= ValueChannel;

    //Find value Channel 6
    result10 = result9.mid(space+1,40);
    for(i=0;i<40;i++)
    {
        if(result10.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    ValueChannel  = findValue(result10.mid(0,space),0);
    ModifySceneConfigurationValue[9]= ValueChannel;

    //Find value Channel 7
    result11 = result10.mid(space+1,40);
    for(i=0;i<40;i++)
    {
        if(result11.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }
    ValueChannel  = findValue(result11.mid(0,space),0);
    ModifySceneConfigurationValue[10]= ValueChannel;
    //Find Scene no
    ValueChannel = findValue(result11.mid(space+1,40),0);
    ModifySceneConfigurationValue[11]= ValueChannel;

}
void SmartBusFuntion::ModifySceneConfigurationUART(char SubnetID, char RTUID, char AreaNo, char SceneNo, char *value)
{
    smartbusF.modifySceneConfiguration(SubnetID,RTUID,AreaNo,SceneNo,0x00,value);

    int j;
    for(j=0;j<smartbusF.lenghtData();j++)
    {
        ModifySceneConfigurationResponse[j]=smartbusF.ReceivedData(j);
        qDebug("%X",ModifySceneConfigurationResponse[j]);
    }
    finishReceived = 1;

}
char SmartBusFuntion::getModifySceneConfigurationValue(int index)
{
    return ModifySceneConfigurationValue[index];

}
char SmartBusFuntion::getModifySceneConfigurationResponse(int index)
{
    return ModifySceneConfigurationResponse[index];

}

//Read status of channel
void SmartBusFuntion::ReadStatusOfChannelTCP(QString resultFromTCP)
{
    allowTransmit=1;
    functionTransmit=READ_STATUS_OF_CHANNEL;


    QString result1 ;
    char SubnetID, RTUID;
    int space;
    int i;
    QString Space=" ";

    //Find Subnet ID
    result1 = resultFromTCP.mid(20,20);
    for(i=0;i<20;i++)
    {
        if(result1.mid(i,1)==Space)
        {
            space = i;
            break;
        }
    }

    SubnetID = findValue(result1.mid(0,space),0);
    ReadStatusOfChannelValue[0] = SubnetID;
    //Find RTUID
    RTUID= findValue(result1.mid(space+1,20),0);
    ReadStatusOfChannelValue[1]= RTUID;

}
void SmartBusFuntion::ReadStatusOfChannelUART(char SubnetID, char RTUID)
{
    smartbusF.readStatusOfChannel(SubnetID, RTUID);


    int j;
    for(j=0;j<smartbusF.lenghtData();j++)
    {
        ReadStatusOfChannelResponse[j]=smartbusF.ReceivedData(j);
        qDebug("%X",ReadStatusOfChannelResponse[j]);
    }
    finishReceived = 1;

}
char SmartBusFuntion::getReadStatusOfChannelValue(int index)
{
    return ReadStatusOfChannelValue[index];

}
char SmartBusFuntion::getReadStatusOfChannelResponse(int index)
{
    return ReadStatusOfChannelResponse[index];

}

//find value TCP
char SmartBusFuntion::findValue(QString temp ,int point)
{
    QString resultTemp;
    QString Space=" ";
    bool conver;
    char result;
    if(temp.mid(point+1,1)==Space)
    {
        //space 2 = 14
        resultTemp = temp.mid(point,1);

    }else
        if(temp.mid(point+2,1)==Space)
        {
            //space 2 =15
            resultTemp = temp.mid(point,2);
        }
        else
        {
            //space 2=16
            resultTemp = temp.mid(point,3);
        }

    result = resultTemp.toInt(&conver,10);

    return result;

}

//////////////////////////////////////////////////////////////
///read wait event random
int eventRandom=0;

int SmartBusFuntion::getNewEvent(void)
{
    return eventRandom;

}
void SmartBusFuntion::setNewEvent(int value)
{
    eventRandom = value;
}

int SmartBusFuntion::getRelayValueEvent(int index)
{
    return RecievedRelay[index];

}

int SmartBusFuntion::getDimmervalueEvent(int index)
{
    return RecievedDimmer[index];
}

void SmartBusFuntion::ReadValueRandom(void)
{
    int j,compairStatus;
    uint16_t Device;
    smartbusF.Received();
    for(j=0;j<smartbusF.lenghtData();j++)
    {
        Recieved[j]=smartbusF.ReceivedData(j);
        //qDebug("%X",Recieved[j]);
    }
    Device = Recieved[3]*0x100 + Recieved[4];
    //qDebug("%X",Device);
    switch(Device)
    {
    case BUTTON_4:
    {
        for(j=0;j<smartbusF.lenghtData();j++)
        {
            RecievedButton[j]=Recieved[j];
        }

        qDebug("Button");
        SubnetIDButton = Recieved[1]; //Subnet ID
        RTUIDButton = Recieved[2]; //RTU ID
        StatusButton = Recieved[13]; //status
//        qDebug("Subnet ID Button %X",SubnetIDButton);
//        qDebug("RTU ID Button %X",RTUIDButton);
//        qDebug("Status Button %X",StatusButton);
        resetBufferReceived();
        break;
    }
    case RELAY_8CHANNEL:
    {
        setNewEvent(1);
        for(j=0;j<smartbusF.lenghtData();j++)
        {
            RecievedRelay[j]=Recieved[j];
        }

        qDebug("Relay");
        SubnetIDRelay = Recieved[1]; //Subnet ID
        RTUIDRelay = Recieved[2]; //RTU ID
        StatusRelay = Recieved[13]; // Status Relay
//        qDebug("Subnet ID Relay %X",SubnetIDRelay);
//        qDebug("RTU ID Relay %X",RTUIDRelay);
//        qDebug("Status Relay %X",StatusRelay);
        for(int j=0;j<8;j++)
        {
            compairStatus = StatusRelay&channelStatus[j];
            qDebug()<<compairStatus;
            if(compairStatus!=0)
            {
                mysql.updateSystemStatus(RTUIDRelay,j+1,100);
            }else
            {
                mysql.updateSystemStatus(RTUIDRelay,j+1,0);
            }
        }

        resetBufferReceived();
        break;
    }
    case DIMMER_8CHANNEL:
    {
        setNewEvent(2);
        for(j=0;j<smartbusF.lenghtData();j++)
        {
            RecievedDimmer[j]=Recieved[j];
        }

        qDebug("Dimmer");
        SubnetIDDimmer = RecievedDimmer[1]; //Subnet ID
        RTUIDDimmer = RecievedDimmer[2]; //RTU ID
        StatusDimmer = RecievedDimmer[11]; // Status Dimer
//        qDebug("Subnet ID %X",SubnetIDDimmer);
//        qDebug("RTU ID %X",RTUIDDimmer);
//        qDebug("Status %X",StatusDimmer);
        for(int j=0;j<8;j++)
        {
            compairStatus = StatusDimmer&channelStatus[j];
            qDebug()<<compairStatus;
            if(compairStatus!=0)
            {
                mysql.updateSystemStatus(RTUIDDimmer,j+1,100);
            }else
            {
                mysql.updateSystemStatus(RTUIDDimmer,j+1,0);
            }
        }

        resetBufferReceived();
        break;
    }
    case 0xFFFE:
    {
        resetBufferReceived();
        break;
    }
    }

}

//Reset buffer
void SmartBusFuntion::resetBufferReceived(void)
{
    int j;
    for(j=0;j<smartbusF.lenghtData();j++)
    {
        Recieved[j]=0;
        SingleChannelControlResponse[j]=-1;
    }
    smartbusF.clrearBufferRec();

}
