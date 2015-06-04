#include "SmartBus.h"
#include "arduPi.h"
#include <QDebug>

char OriginalSubnetID;
char OriginalDeviceID;
int  OriginaldeviceType;
char QTYOfChannel;


SmartBusRas::SmartBusRas(void)
{
    _u8SerialPort = 0;
}


void SmartBusRas::setOriginalID(char SetOriginalSubnetID, char SetOriginalDeviceID, int SetOriginaldeviceType, char SetQTYOfChannel)
{
    OriginalSubnetID = SetOriginalSubnetID;
    OriginalDeviceID = SetOriginalDeviceID;
    OriginaldeviceType = SetOriginaldeviceType;
    QTYOfChannel = SetQTYOfChannel;

}

////////////////////////////////////////////////////////////////////////////////
// CRC Calculator
// Input: ot including Start code 0xAA,0xAA
////////////////////////////////////////////////////////////////////////////////
void SmartBusRas::Pack_crc(char *ptr, char len)
{
    uint16_t  crc;
    char dat;
    crc=0;
    while(len--!=0)
    {
        dat=crc>>8;               /* */
        crc<<=8;                  /*  */
        crc^=CRC_TAB[dat^*ptr];   /* */
        ptr++;
    }
    *ptr=crc>>8; // high byte
    ptr++;
    *ptr=crc&0xff; // low byte
}

char SmartBusRas::Check_crc(char *ptr, char len)
{
    uint16_t crc;
    char dat,Hcrc;
    crc=0;
    while(len--!=0)
    {
        dat=crc>>8;               /* */
        crc<<=8;                  /* */
        crc^=CRC_TAB[dat^*ptr];   /* */
        ptr++;
    }
    dat=crc&0xff;
    Hcrc = crc>>8;
    if((*ptr==Hcrc)&&(*(ptr+1)==dat))
        return 1;
    else
        return 0;

}

void SmartBusRas::RCV_ON()
{
    digitalWrite(LED, HIGH);
}

void SmartBusRas::RCV_OFF()
{
    digitalWrite(LED, LOW);
}

void SmartBusRas::SmartBus_init(int baud)
{
    Serial.begin(baud);
    RCV_OFF(); //enable received

}

////////////////////////////////////////////////////////////////////////////////
// Purpose:    Puts a character onto the serial line
// Inputs:     Character
// Outputs:    None
////////////////////////////////////////////////////////////////////////////////
void SmartBusRas::Sbus_serial_putc(char c)
{
    Serial.print(c);
}
void SmartBusRas::Sbus_serial_send_start(void)
{
    RCV_ON(); //enable Transmit
    Sbus_serial_putc(0xAA);//Start code
    Sbus_serial_putc(0xAA);//Start code
}
void SmartBusRas::Sbus_serial_send_stop(int mode)
{
    if(mode==1)
    {
        delay(20);
    }else
        if(mode==5)
        {
            delay(15);
        }
        else
            if(mode==2)
            {
                delay(30);
            }else
                delay(40);

    RCV_OFF(); //enable Received
    Received();
}

/*SBUS Master Functions*/
////////////////////////////////////////////////////////////////////////////////
// Purpose:    Send a message over the RS485 bus
// Inputs:     1) TargetSubnetID, TargetDeviceID: The destination address
//             2) ChannelNo: The number of bytes of channel
//             3) BrightnessLevel: The Brightness Level 0 is off, 100 is on
//             4) Runningtime: The Running time 0-3600s
////////////////////////////////////////////////////////////////////////////////
//Detect Address
void SmartBusRas::detectAddress(void)
{
    char BuffTranfer[18];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_DETECT_ADDRESS>>8;//FUNC_DETECT_ADDRESS Hbyte
    BuffTranfer[6] = FUNC_DETECT_ADDRESS&0xff;//FUNC_DETECT_ADDRESS Lbyte
    BuffTranfer[7] = 0xFF;//TargetSubnetID
    BuffTranfer[8] = 0xFF;//TargetDeviceID

    Pack_crc(&BuffTranfer[0],9);  // tinh ccr bat deu tu vi tri BuffTranfer[0]
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); //transmit data
    Sbus_serial_send_stop(1);

}

//Modify Address
void SmartBusRas::modifyAddress(char Target_SubnetID, char Target_DeviceID, char NewSubnetID, char NewDeviceID)
{
    char BuffTranfer[18];
    int i;
    BuffTranfer[0] = 13;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_MODIFY_ADDRESS>>8;//FUNC_MODIFY_ADDRESS Hbyte
    BuffTranfer[6] = FUNC_MODIFY_ADDRESS&0xff;//FUNC_MODIFY_ADDRESS Lbyte
    BuffTranfer[7] = Target_SubnetID;//TargetSubnetID
    BuffTranfer[8] = Target_DeviceID;//TargetDeviceID
    BuffTranfer[9] = NewSubnetID; //Subnet ID of target device
    BuffTranfer[10]= NewDeviceID; //Device ID of target device

    Pack_crc(&BuffTranfer[0],11);  // tinh ccr bat deu tu vi tri BuffTranfer[0]
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); //transmit data
    Sbus_serial_send_stop(1);

}

//Single channel control
void SmartBusRas::singleChannelControl(char Target_SubnetID, char Target_DeviceID, char Channel_No, char Brightness_Level, uint16_t Running_Time)
{
    char BuffTranfer[18];
    int i;
    BuffTranfer[0] = 15;//15;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_SINGLE_CHANNEL_CONTROL>>8;//FUNC_SINGLE_CHANNEL_CONTROL Hbyte
    BuffTranfer[6] = FUNC_SINGLE_CHANNEL_CONTROL&0xff;//FUNC_SINGLE_CHANNEL_CONTROL Lbyte
    BuffTranfer[7] = Target_SubnetID;//TargetSubnetID
    BuffTranfer[8] = Target_DeviceID;//TargetDeviceID
    BuffTranfer[9] = Channel_No;//ChannelNo
    BuffTranfer[10] = Brightness_Level;//BrightnessLevel
    BuffTranfer[11] = Running_Time>>8;//Runningtime Hbyte
    BuffTranfer[12] = Running_Time&0xff;//Runningtime Lbyte
    //BuffTranfer[13] = Button_No;//ButtonNo

    Pack_crc(&BuffTranfer[0],13);
    // Serial.flush();
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)
    {
        Serial.print(BuffTranfer[i]);//Transmit data
    }
//    Serial.print("xin chao arduino!\n");
    Sbus_serial_send_stop(1);
    Received();

}


//Scene controll
void SmartBusRas::sceneControl(char Target_SubnetID, char Target_DeviceID, char Zone_No, char Scene_No)
{
    char BuffTranfer[18];
    int i;
    BuffTranfer[0] = 13;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_SCENE_CONTROL>>8;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Hbyte
    BuffTranfer[6] = FUNC_SCENE_CONTROL&0xff;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    BuffTranfer[9] = Zone_No;//Zone_No
    BuffTranfer[10] = Scene_No;//Scene_No

    Pack_crc(&BuffTranfer[0],11);
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)
        Serial.print(BuffTranfer[i]);//transmit data
    Serial.print("\n");
    Sbus_serial_send_stop(1);

}


//Sequence Control
void SmartBusRas::sequenceControl(char Target_SubnetID, char Target_DeviceID, char Area_No, char Sequence_No)
{
    char BuffTranfer[18];
    int i;
    BuffTranfer[0] = 13;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_SEQUENCE_CONTROL>>8;//FUNC_READ_STATUS_OF_CHANNEL_RESPONSE Hbyte
    BuffTranfer[6] = FUNC_SEQUENCE_CONTROL&0xff;//FUNC_READ_STATUS_OF_CHANNEL_RESPONSE Lbyte
    BuffTranfer[7] = Target_SubnetID;//TargetSubnetID
    BuffTranfer[8] = Target_DeviceID;//TargetDeviceID
    BuffTranfer[9] = Area_No; //Area No
    BuffTranfer[10]= Sequence_No; //Sequence No

    Pack_crc(&BuffTranfer[0],11);  // tinh ccr bat deu tu vi tri BuffTranfer[0]
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); //transmit data
    Sbus_serial_send_stop(1);

}

//Read status of channel
void SmartBusRas::readStatusOfChannel(char Target_SubnetID, char Target_DeviceID)//, unsigned int16 Status_of_Channel2);
{
    char BuffTranfer[18];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_STATUS_OF_CHANNEL>>8;//FUNC_READ_STATUS_OF_CHANNEL_RESPONSE Hbyte
    BuffTranfer[6] = FUNC_READ_STATUS_OF_CHANNEL&0xff;//FUNC_READ_STATUS_OF_CHANNEL_RESPONSE Lbyte
    BuffTranfer[7] = Target_SubnetID;//TargetSubnetID
    BuffTranfer[8] = Target_DeviceID;//TargetDeviceID

    Pack_crc(&BuffTranfer[0],9);  // tinh ccr bat deu tu vi tri BuffTranfer[0]
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); //transmit data
    Sbus_serial_send_stop(5);

}

//Reversing Control
void SmartBusRas::reversingControl(char Target_SubnetID, char Target_DeviceID, char ChannelNo, char Reserved, uint16_t RunningTime)
{
    char BuffTranfer[18];
    int i;
    BuffTranfer[0] = 0x0F;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_REVERSING_CONTROL>>8;//FUNC_READ_STATUS_OF_CHANNEL_RESPONSE Hbyte
    BuffTranfer[6] = FUNC_REVERSING_CONTROL&0xff;//FUNC_READ_STATUS_OF_CHANNEL_RESPONSE Lbyte
    BuffTranfer[7] = Target_SubnetID;//TargetSubnetID
    BuffTranfer[8] = Target_DeviceID;//TargetDeviceID
    BuffTranfer[9] = ChannelNo; //Channel No
    BuffTranfer[10]= Reserved; //Reserved
    BuffTranfer[11] = RunningTime>>8;//Runningtime Hbyte
    BuffTranfer[12] = RunningTime&0xff;//Runningtime Lbyte

    Pack_crc(&BuffTranfer[0],13);  // tinh ccr bat deu tu vi tri BuffTranfer[0]
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); //transmit data
    Sbus_serial_send_stop(1);
}

//Forwardly	Report Status
void SmartBusRas::forwardlyReportStatus(char QTYOfZones,char StatusOfZone1, char StatusOfZone2, char QTYOfChannel)
{
    char BuffTranfer[18];
    int i;
    BuffTranfer[0] = 0x0F;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_FORWARDLY_REPORT_STATUS>>8;//FUNC_FORWARDLY_REPORT_STATUS Hbyte
    BuffTranfer[6] = FUNC_FORWARDLY_REPORT_STATUS&0xff;//FUNC_FORWARDLY_REPORT_STATUS Lbyte
    BuffTranfer[7] = 0xFF;//TargetSubnetID
    BuffTranfer[8] = 0xFF;//TargetDeviceID
    BuffTranfer[9] = QTYOfZones; //QTY of zones
    BuffTranfer[10]= StatusOfZone1; //Status of zone 1
    BuffTranfer[11] = StatusOfZone2;//Status of zone 2
    BuffTranfer[12] = QTYOfChannel;

    Pack_crc(&BuffTranfer[0],13);  // tinh ccr bat deu tu vi tri BuffTranfer[0]
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); //transmit data
    Sbus_serial_send_stop(1);

}

//Read scene configuration
void SmartBusRas::readSceneConfiguration(char Target_SubnetID, char Target_DeviceID, char AreaNo, char SceneNo)
{
    char BuffTranfer[18];
    int i;
    BuffTranfer[0] = 13;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_SCENE_CONFIGURATION>>8;//FUNC_READ_SCENE_CONFIGURATION Hbyte
    BuffTranfer[6] = FUNC_READ_SCENE_CONFIGURATION&0xff;//FUNC_READ_SCENE_CONFIGURATION Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    BuffTranfer[9] = AreaNo;//Area no
    BuffTranfer[10] = SceneNo;//Scene_No

    Pack_crc(&BuffTranfer[0],11);
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(1);

}

//Modify scene configuration
void SmartBusRas::modifySceneConfiguration(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char SceneNo, uint16_t RunningTimeScene, char *ChannelIntensity)
{
    char BuffTranfer[25];
    int i;
    BuffTranfer[0] = 23;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_MODIFY_SCENE_CONFIGURATION>>8;//FUNC_MODIFY_SCENE_CONFIGURATION Hbyte
    BuffTranfer[6] = FUNC_MODIFY_SCENE_CONFIGURATION&0xff;//FUNC_MODIFY_SCENE_CONFIGURATION Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    BuffTranfer[9] = ZoneNo;//Zone_No
    BuffTranfer[10] = SceneNo;//Scene_No
    BuffTranfer[11] = RunningTimeScene>>8;//Running_Time_Scene Hbyte
    BuffTranfer[12] = RunningTimeScene&0xff;//Running_Time_Scene Lbyte

    for(i=0;i<8;i++)
        BuffTranfer[13+i] = ChannelIntensity[i];

    Pack_crc(&BuffTranfer[0],21);
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(2);

}

//Curtain control enabled
void SmartBusRas::curtainControlEnabled(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[14];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_CURTAIN_CONTROL_ENABLED>>8;//FUNC_CURTAIN_CONTROL_ENABLED Hbyte
    BuffTranfer[6] = FUNC_CURTAIN_CONTROL_ENABLED&0xff;//FUNC_CURTAIN_CONTROL_ENABLED Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    Pack_crc(&BuffTranfer[0],9); // 11 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(1);

}

//Read remark of zone
void SmartBusRas::readRemarkOfZone(char Target_SubnetID, char Target_DeviceID, char ZoneNumber)
{
    char BuffTranfer[14];
    int i;
    BuffTranfer[0] =12;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_REMARK_OF_ONE_ZONE>>8;//FUNC_READ_REMARK_OF_ONE_ZONE Hbyte
    BuffTranfer[6] = FUNC_READ_REMARK_OF_ONE_ZONE&0xff;//FUNC_READ_REMARK_OF_ONE_ZONE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    BuffTranfer[9] = ZoneNumber;//

    Pack_crc(&BuffTranfer[0],10); // 12 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); //transmit data
    Sbus_serial_send_stop(1);

}

//Write remark of zone
void SmartBusRas::writeRemarkOfZone(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char *ZoneRemark)
{
    char BuffTranfer[34];
    int i;
    BuffTranfer[0] =32;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_WRITE_REMARK_OF_ONE_ZONE>>8;//FUNC_WRITE_REMARK_OF_ONE_ZONE Hbyte
    BuffTranfer[6] = FUNC_WRITE_REMARK_OF_ONE_ZONE&0xff;//FUNC_WRITE_REMARK_OF_ONE_ZONE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    BuffTranfer[9] = ZoneNo;//

    for(i=0;i<20;i++)
        BuffTranfer[10+i] = ZoneRemark[i];
    /////////////////////////////////////////////
    Pack_crc(&BuffTranfer[0],30); // 30 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); //transmit data
    Sbus_serial_send_stop(2);
}

//Make zone of relay
void SmartBusRas::makeZoneOfRelay(char Target_SubnetID, char Target_DeviceID,char ZoneNo, char FlagChannel1, char FlagChannel2, char MaxChannel)
{
    char BuffTranfer[20];
    int i;
    BuffTranfer[0] = 17;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_MAKE_ZONE_OF_RELAY>>8;//FUNC_MAKE_ZONE_OF_RELAY Hbyte
    BuffTranfer[6] = FUNC_MAKE_ZONE_OF_RELAY&0xff;//FUNC_MAKE_ZONE_OF_RELAY Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID
    BuffTranfer[9] = 0x00;//Invalid
    BuffTranfer[10] = 0x00;//Invalid
    BuffTranfer[11] = ZoneNo;//Zone no
    BuffTranfer[12] = FlagChannel1;//Flag for channel 1
    BuffTranfer[13] = FlagChannel2;//Flag for channel 2
    BuffTranfer[14] = MaxChannel; //Flag for channel max
    Pack_crc(&BuffTranfer[0],15); // 17 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(2);

}


//Read channel remark
void SmartBusRas::readChannelRemark(char Target_SubnetID, char Target_DeviceID, char ChannelNo)
{
    char BuffTranfer[14];
    int i;
    BuffTranfer[0] = 12;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_CHANNEL_REMARK>>8;//FUNC_READ_CHANNEL_REMARK Hbyte
    BuffTranfer[6] = FUNC_READ_CHANNEL_REMARK&0xff;//FUNC_READ_CHANNEL_REMARK Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID
    BuffTranfer[9] = ChannelNo ;// Channel no

    Pack_crc(&BuffTranfer[0],10); // 12 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(1);
}

//Write channel remark
void SmartBusRas::writeChannelRemark(char Target_SubnetID, char Target_DeviceID, char ChannelNo, char *ChannelRemark)
{
    char BuffTranfer[34];
    int i;
    BuffTranfer[0] =32;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_WRITE_CHANNEL_REMARK>>8;//FUNC_WRITE_CHANNEL_REMARK Hbyte
    BuffTranfer[6] = FUNC_WRITE_CHANNEL_REMARK&0xff;//FUNC_WRITE_CHANNEL_REMARK Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    BuffTranfer[9] = ChannelNo;//

    for(i=0;i<20;i++)
        BuffTranfer[10+i] = ChannelRemark[i];
    /////////////////////////////////////////////
    Pack_crc(&BuffTranfer[0],30); // 30 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(3);
}

//Read channel load type
void SmartBusRas::readChannelLoadType(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[14];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_CHANNEL_LOAD_TYPE>>8;//FUNC_READ_CHANNEL_LOAD_TYPE Hbyte
    BuffTranfer[6] = FUNC_READ_CHANNEL_LOAD_TYPE&0xff;//FUNC_READ_CHANNEL_LOAD_TYPE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    Pack_crc(&BuffTranfer[0],9); // 11 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);// transmit data
    Sbus_serial_send_stop(1);
}

//Modify channel load type
void SmartBusRas::modifyChannelLoadType(char Target_SubnetID, char Target_DeviceID, char *ChannelLoadType)
{
    char BuffTranfer[24];
    int i;
    BuffTranfer[0] = 19;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_MODIFY_CHANNEL_LOAD_TYPE>>8;//FUNC_MODIFY_CHANNEL_LOAD_TYPE Hbyte
    BuffTranfer[6] = FUNC_MODIFY_CHANNEL_LOAD_TYPE&0xff;//FUNC_MODIFY_CHANNEL_LOAD_TYPE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID
    for(i=0;i<8;i++)
        BuffTranfer[9+i] = ChannelLoadType[i];//Status of Channel i

    Pack_crc(&BuffTranfer[0],17); // 17 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//data transmit
    Sbus_serial_send_stop(2);
}


//Read delay of turn on channel
void SmartBusRas::readDelayOfTurnOnChannel(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[14];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_DELAY_OF_TURN_ON_CHANNEL>>8;//FUNC_READ_DELAY_OF_TURN_ON_CHANNEL Hbyte
    BuffTranfer[6] = FUNC_READ_DELAY_OF_TURN_ON_CHANNEL&0xff;//FUNC_READ_DELAY_OF_TURN_ON_CHANNEL Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    Pack_crc(&BuffTranfer[0],9); // 11 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); //transmit data
    Sbus_serial_send_stop(1);
}

//Modify delay of turn on channel
void SmartBusRas::modifyDelayOfTurnOnChannel(char Target_SubnetID, char Target_DeviceID, char *DelayOfTurnOnChannel)
{
    char BuffTranfer[24];
    int i;
    BuffTranfer[0] = 19;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_MODIFY_DELAY_OF_TURN_ON_CHANNEL>>8;//FUNC_MODIFY_DELAY_OF_TURN_ON_CHANNEL Hbyte
    BuffTranfer[6] = FUNC_MODIFY_DELAY_OF_TURN_ON_CHANNEL&0xff;//FUNC_MODIFY_DELAY_OF_TURN_ON_CHANNEL Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID
    for(i=0;i<8;i++)
        BuffTranfer[9+i] = DelayOfTurnOnChannel[i];//Status of Channel i

    Pack_crc(&BuffTranfer[0],17); // 17 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); //transmit data
    Sbus_serial_send_stop(2);
}


//Read safeguard time of channel
void SmartBusRas::readSafeguardTimeOfChannel(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[14];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_SAFEGUARD_TIME_OF_CHANNEL>>8;//FUNC_READ_SAFEGUARD_TIME_OF_CHANNEL Hbyte
    BuffTranfer[6] = FUNC_READ_SAFEGUARD_TIME_OF_CHANNEL&0xff;//FUNC_READ_SAFEGUARD_TIME_OF_CHANNEL Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    Pack_crc(&BuffTranfer[0],9); // 11 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(1);
}

//Modify safeguard time of channel
void SmartBusRas::modifySafeguardTimeOfChannel(char Target_SubnetID, char Target_DeviceID, char *SafeguardTimeOfChannel)
{
    char BuffTranfer[24];
    int i;
    BuffTranfer[0] = 19;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_MODIFY_SAFEGUARD_TIME_OF_CHANNEL>>8;//FUNC_MODIFY_SAFEGUARD_TIME_OF_CHANNEL Hbyte
    BuffTranfer[6] = FUNC_MODIFY_SAFEGUARD_TIME_OF_CHANNEL&0xff;//FUNC_MODIFY_SAFEGUARD_TIME_OF_CHANNEL Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID
    for(i=0;i<8;i++)
        BuffTranfer[9+i] = SafeguardTimeOfChannel[i];//Status of Channel i

    Pack_crc(&BuffTranfer[0],17); // 19 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(2);
}

//Read setting of zones
void SmartBusRas::readSettingOfZone(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[14];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_SETTING_OF_ZONE>>8;//FUNC_READ_SETTING_OF_ZONE Hbyte
    BuffTranfer[6] = FUNC_READ_SETTING_OF_ZONE&0xff;//FUNC_READ_SETTING_OF_ZONE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    Pack_crc(&BuffTranfer[0],9); // 11 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(1);
}

//Read remark of specify scene of specify zone
void SmartBusRas::readRemarkOfSpecifySceneOfSpecifyZone(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[15];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_REMARK_OF_SPECIFY_SCENE_OF_ZONE>>8;//FUNC_READ_REMARK_OF_SPECIFY_SCENE_OF_ZONE Hbyte
    BuffTranfer[6] = FUNC_READ_REMARK_OF_SPECIFY_SCENE_OF_ZONE&0xff;//FUNC_READ_REMARK_OF_SPECIFY_SCENE_OF_ZONE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    Pack_crc(&BuffTranfer[0],9);
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); // Transmit data
    Sbus_serial_send_stop(1);
}

//Write remark of specify scene of specify zone
void SmartBusRas::writeRemarkOfSpecifySceneOfSpecifyZone(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char SceneNo, char *RemarkOfCurrentScene)
{
    char BuffTranfer[35];
    int i;
    BuffTranfer[0] = 33;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_WRITE_REMARK_OF_SPECIFY_SCENE_OF_ZONE>>8;//FUNC_WRITE_REMARK_OF_SPECIFY_SCENE_OF_ZONE Hbyte
    BuffTranfer[6] = FUNC_WRITE_REMARK_OF_SPECIFY_SCENE_OF_ZONE&0xff;//FUNC_WRITE_REMARK_OF_SPECIFY_SCENE_OF_ZONE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    BuffTranfer[9] = ZoneNo;//Zone_No
    BuffTranfer[10] = SceneNo;//Scene_No
    for(i=0;i<20;i++)
        BuffTranfer[11+i] = RemarkOfCurrentScene[i];

    Pack_crc(&BuffTranfer[0],31);
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); //transmit data
    Sbus_serial_send_stop(2);
}


//Read type of zone when power on
void SmartBusRas::readTypeOfZoneWhenPowerOn(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[15];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_TYPE_OF_ZONE_WHEN_POWER_ON>>8;//FUNC_READ_TYPE_OF_ZONE_WHEN_POWER_ON Hbyte
    BuffTranfer[6] = FUNC_READ_TYPE_OF_ZONE_WHEN_POWER_ON&0xff;//FUNC_READ_TYPE_OF_ZONE_WHEN_POWER_ON Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    Pack_crc(&BuffTranfer[0],9);
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); // Transmit data
    Sbus_serial_send_stop(1);
}

//Write type of zone when power on
void SmartBusRas::writeTypeOfZoneWhenPowerOn(char Target_SubnetID, char Target_DeviceID, char *TypeOfZoneWhenPowerOn)
{
    char BuffTranfer[24];
    int i;
    BuffTranfer[0] = 19;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_WRITE_TYPE_OF_ZONE_WHEN_POWER_ON>>8;//FUNC_WRITE_TYPE_OF_ZONE_WHEN_POWER_ON Hbyte
    BuffTranfer[6] = FUNC_WRITE_TYPE_OF_ZONE_WHEN_POWER_ON&0xff;//FUNC_WRITE_TYPE_OF_ZONE_WHEN_POWER_ON Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID
    for(i=0;i<8;i++)
        BuffTranfer[9+i] = TypeOfZoneWhenPowerOn[i];//Status of Channel i

    Pack_crc(&BuffTranfer[0],17); // 17 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(2);
}

//Read scene No. of every zone when power on
void SmartBusRas::readSceneNoOfEveryZoneWhenPowerOn(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[15];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON>>8;//FUNC_READ_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON Hbyte
    BuffTranfer[6] = FUNC_READ_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON&0xff;//FUNC_READ_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    Pack_crc(&BuffTranfer[0],9);
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]); // Transmit data
    Sbus_serial_send_stop(1);

}


//Modify scene No. of every zone when power on
void SmartBusRas::modifySceneNoOfEveryZoneWhenPowerOn(char Target_SubnetID, char Target_DeviceID, char *SceneNoOfEveryZoneWhenPowerOn)
{
    char BuffTranfer[24];
    int i;
    BuffTranfer[0] = 19;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_MODIFY_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON>>8;//FUNC_MODIFY_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON Hbyte
    BuffTranfer[6] = FUNC_MODIFY_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON&0xff;//FUNC_MODIFY_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID
    for(i=0;i<8;i++)
        BuffTranfer[9+i] = SceneNoOfEveryZoneWhenPowerOn[i];//Scene No i

    Pack_crc(&BuffTranfer[0],17); // 17 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(2);

}


//Read device remark
void SmartBusRas::readDeviceRemark(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[18];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_DEVICE_REMARK>>8;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Hbyte
    BuffTranfer[6] = FUNC_READ_DEVICE_REMARK&0xff;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    Pack_crc(&BuffTranfer[0],9);
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);// transmit data
    Sbus_serial_send_stop(1);

}


//Read filmwave version

//Read mac address
void SmartBusRas::readMacAddress(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[18];
    int i;
    BuffTranfer[0] = 11;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_READ_MAC_ADDRESS>>8;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Hbyte
    BuffTranfer[6] = FUNC_READ_MAC_ADDRESS&0xff;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    Pack_crc(&BuffTranfer[0],9); // 21 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(1);

}

//Modify ID by mac address
void SmartBusRas::modifyIDByMacAddress(char Target_SubnetID, char Target_DeviceID, char *MacAddress, char SubnetChange, char DeviceChange)
{
    char BuffTranfer[26];
    int i;
    BuffTranfer[0] = 21;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_MODIFY_ID_BY_MAC_ADDRESS>>8;//FUNC_MODIFY_ID_BY_MAC_ADDRESS Hbyte
    BuffTranfer[6] = FUNC_MODIFY_ID_BY_MAC_ADDRESS&0xff;//FUNC_MODIFY_ID_BY_MAC_ADDRESS Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID
    for(i=0;i<8;i++)
        BuffTranfer[9+i] = MacAddress[i];//Status of Channel i
    BuffTranfer[17] = SubnetChange;
    BuffTranfer[18] = DeviceChange;

    Pack_crc(&BuffTranfer[0],19); // 21 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(2);

}


//Modify address button
void SmartBusRas::modifyAddressButtonResponse(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[14];
    int i;
    BuffTranfer[0] = 12;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_MODIFY_ADDRESS_BUTTON_RESPONSE>>8;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Hbyte
    BuffTranfer[6] = FUNC_MODIFY_ADDRESS_BUTTON_RESPONSE&0xff;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID
    BuffTranfer[9] = 0xF8 ;// ok

    Pack_crc(&BuffTranfer[0],10); // 12 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);//transmit data
    Sbus_serial_send_stop(1);

}

//Detect Address button
void SmartBusRas::detectAddressButtonResponse(char Target_SubnetID, char Target_DeviceID)
{
    char BuffTranfer[15];
    int i;
    BuffTranfer[0] = 13;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_DETECT_ADDRESS_BUTTON_RESPONSE>>8;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Hbyte
    BuffTranfer[6] = FUNC_DETECT_ADDRESS_BUTTON_RESPONSE&0xff;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    BuffTranfer[9] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[10] = OriginalDeviceID;//OriginalDeviceID

    Pack_crc(&BuffTranfer[0],11); // 11 byte
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);// transmit data
    Sbus_serial_send_stop(1);

}

//Write device remark
void SmartBusRas::writeDeviceRemark(char Target_SubnetID, char Target_DeviceID, char *DeviceRemark)
{
    char BuffTranfer[33];
    int i;
    BuffTranfer[0] = 0x1F;//Length
    BuffTranfer[1] = OriginalSubnetID;//OriginalSubnetID
    BuffTranfer[2] = OriginalDeviceID;//OriginalDeviceID
    BuffTranfer[3] = OriginaldeviceType>>8;//OriginaldeviceType Hbyte
    BuffTranfer[4] = OriginaldeviceType&0xff;//OriginaldeviceType Lbyte
    BuffTranfer[5] = FUNC_WRITE_DEVICE_REMARK>>8;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Hbyte
    BuffTranfer[6] = FUNC_WRITE_DEVICE_REMARK&0xff;//FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE Lbyte
    BuffTranfer[7] = Target_SubnetID;//Target_SubnetID
    BuffTranfer[8] = Target_DeviceID;//Target_DeviceID

    for(i=0;i<20;i++)
        BuffTranfer[9+i] = DeviceRemark[i];

    Pack_crc(&BuffTranfer[0],29);
    Sbus_serial_send_start();
    for(i=0;i<BuffTranfer[0];i++)// BuffTranfer[0] Length
        Serial.print(BuffTranfer[i]);// transmit data
    Sbus_serial_send_stop(3);
}

//Read specify device on line



void SmartBusRas::Received(void)
{
    char val = 0xFF;
    int cont=0,i,StartCode, Length;
    char DataRec[88];
    StartCode = 0;
    val = 0;
    cont = 0;
    Length=0;
    //Find start code = 0xAA
    while((val!= 0xAA)&&(cont<100))
    {
        if(Serial.available())
        {
            val = Serial.read();

        }
        delay(5);
        cont ++;
        if(val!=0)
        qDebug("%d",val);
    }
    //qDebug("%X",val);
    StartCode=1;
    if(Serial.available())
    {
        val = Serial.read();
        if(val == 0xAA)
            StartCode=2;
    }
    //qDebug("%X",val);
    if(StartCode==2)
    {
        if(Serial.available())
        {
            val = Serial.read();
            Length = val;
            DataRec[0]= Length;
            //qDebug("%X", DataRec[0]);
        }
        for(i=1;i<Length; i++)
        {
            if(Serial.available())
            {
                DataRec[i] = Serial.read();
                //qDebug("%X",DataRec[i]);
                delay(1);
            }
        }
    }
    if(Check_crc(&DataRec[0],Length-2)==TRUE)
    {
        for(i=0;i<Length;i++)
        {
            BufferRec[i] = DataRec[i];
            qDebug("%X",BufferRec[i]);
        }

    }

}

uint16_t SmartBusRas::ReceivedData(int index)
{

    return BufferRec[index];
}

char SmartBusRas::lenghtData(void)
{
    return BufferRec[0];
}

void SmartBusRas::clrearBufferRec(void)
{
    int i;
    for(i=0;i<60;i++)
        BufferRec[i]=0;
}
