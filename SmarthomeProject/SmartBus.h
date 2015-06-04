
#include "arduPi.h"
#include <qglobal.h>


#ifndef SMARTBUS_H
#define SMARTBUS_H

#define uint16_t unsigned int


////////////////////////////////////////////////////////////////////////////////
// Define
////////////////////////////////////////////////////////////////////////////////
typedef enum _function{
    /*Detect Address*/
    FUNC_DETECT_ADDRESS=0xE5F5,
    FUNC_DETECT_ADDRESS_RESPONSE=0xE5F6,
    /*Modify Address*/
    FUNC_MODIFY_ADDRESS=0xE5F7,
    FUNC_MODIFY_ADDRESS_RESPONSE=0xE5F8,

    /*Single channel control*/
    FUNC_SINGLE_CHANNEL_CONTROL=0x0031,
    FUNC_SINGLE_CHANNEL_CONTROL_RESPONSE=0x0032,
    /*Scene control*/
    FUNC_SCENE_CONTROL=0x0002,
    FUNC_SCENE_CONTROL_RESPONSE=0x0003,
    /*Sequence control*/
    FUNC_SEQUENCE_CONTROL=0x001A,
    FUNC_SEQUENCE_CONTROL_RESPONSE=0x001B,
    /*Read status of channel*/
    FUNC_READ_STATUS_OF_CHANNEL=0x0033,
    FUNC_READ_STATUS_OF_CHANNEL_RESPONSE=0x0034,
    /*Reversing control*/
    FUNC_REVERSING_CONTROL=0xDC1C,
    FUNC_REVERSING_CONTROL_RESPONSE=0xDC1D,
    /*Curtain control enabled*/
    FUNC_CURTAIN_CONTROL_ENABLED=0xDC23,
    FUNC_CURTAIN_CONTROL_ENABLED_RESPONSE=0xDC24,
    /*Read Remark of one zone*/
    FUNC_READ_REMARK_OF_ONE_ZONE=0xF00A,
    FUNC_READ_REMARK_OF_ONE_ZONE_RESPONSE=0xF00B,
    /*Write remark of one zone*/
    FUNC_WRITE_REMARK_OF_ONE_ZONE=0xF00C,
    FUNC_WRITE_REMARK_OF_ONE_ZONE_RESPONSE=0xF00D,
    /*Make zones of relay*/
    FUNC_MAKE_ZONE_OF_RELAY=0x0006,
    FUNC_MAKE_ZONE_OF_RELAY_RESPONSE=0x0007,
    /*Read channel remark*/
    FUNC_READ_CHANNEL_REMARK=0xF00E,
    FUNC_READ_CHANNEL_REMARK_RESPONSE=0xF00F,
    /*Write channel remark*/
    FUNC_WRITE_CHANNEL_REMARK=0xF010,
    FUNC_WRITE_CHANNEL_REMARK_RESPONSE=0xF011,
    /*Read channel load type*/
    FUNC_READ_CHANNEL_LOAD_TYPE=0xF012,
    FUNC_READ_CHANNEL_LOAD_TYPE_RESPONSE=0xF013,
    /*Modify channel load type*/
    FUNC_MODIFY_CHANNEL_LOAD_TYPE=0xF014,
    FUNC_MODIFY_CHANNEL_LOAD_TYPE_RESPONSE=0xF015,
    /*Read delay of turn on channel*/
    FUNC_READ_DELAY_OF_TURN_ON_CHANNEL=0xF04D,
    FUNC_READ_DELAY_OF_TURN_ON_CHANNEL_RESPONSE=0xF04E,
    /*Modify delay of turn on channel*/
    FUNC_MODIFY_DELAY_OF_TURN_ON_CHANNEL=0xF04F,
    FUNC_MODIFY_DELAY_OF_TURN_ON_CHANNEL_RESPONSE=0xF050,
    /*Read safeguard time of channel*/
    FUNC_READ_SAFEGUARD_TIME_OF_CHANNEL=0xF03F,
    FUNC_READ_SAFEGUARD_TIME_OF_CHANNEL_RESPONSE=0xF040,
    /*Modify safeguarb time of channel*/
    FUNC_MODIFY_SAFEGUARD_TIME_OF_CHANNEL=0xF041,
    FUNC_MODIFY_SAFEGUARD_TIME_OF_CHANNEL_RESPONSE=0xF042,
    /*Read setting of zone*/
    FUNC_READ_SETTING_OF_ZONE=0x0004,
    FUNC_READ_SETTING_OF_ZONE_RESPONSE=0x0005,
    /*Read remark of specify scene of zone*/
    FUNC_READ_REMARK_OF_SPECIFY_SCENE_OF_ZONE=0xF024,
    FUNC_READ_REMARK_OF_SPECIFY_SCENE_OF_ZONE_RESPONSE=0xF025,
    /*Write remark of specify scene of zone*/
    FUNC_WRITE_REMARK_OF_SPECIFY_SCENE_OF_ZONE=0xF026,
    FUNC_WRITE_REMARK_OF_SPECIFY_SCENE_OF_ZONE_RESPONSE=0xF027,
    /*Read type of zone when power on*/
    FUNC_READ_TYPE_OF_ZONE_WHEN_POWER_ON=0xF051,
    FUNC_READ_TYPE_OF_ZONE_WHEN_POWER_ON_RESPONSE=0xF052,
    /*Write type of zone when power on*/
    FUNC_WRITE_TYPE_OF_ZONE_WHEN_POWER_ON=0xF053,
    FUNC_WRITE_TYPE_OF_ZONE_WHEN_POWER_ON_RESPONSE=0xF054,
    /*Read scene No. of every zone when power on*/
    FUNC_READ_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON=0xF055,
    FUNC_READ_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON_RESPONSE=0xF056,
    /*Modify scene No. of every zone when power on*/
    FUNC_MODIFY_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON=0xF057,
    FUNC_MODIFY_SCENE_NOof_EVERY_ZONE_WHEN_POWER_ON_RESPONSE=0xF058,
    /*Read remark of specify sequence*/
    FUNC_READ_REMARK_OF_SPECIFY_SEQUENCE=0xF028,
    FUNC_READ_REMARK_OF_SPECIFY_SEQUENCE_RESPONSE=0xF029,
    /*Modify remark of specify sequence*/
    FUNC_MODIFY_REMARK_OF_SPECIFY_SEQUENCE=0xF030,
    FUNC_MODIFY_REMARK_OF_SPECIFY_SEQUENCE_RESPONSE=0xF031,
    /*Read setting of sequence running*/
    FUNC_READ_SETTING_OF_SEQUENCE_RUNNING=0x0012,
    FUNC_READ_SETTING_OF_SEQUENCE_RUNNING_RESPONSE=0x0013,
    /*Modify setting of sequence running*/
    FUNC_MODIFY_SETING_OF_SEQUENCE_RUNNING=0x0018,
    FUNC_MODIFY_SETING_OF_SEQUENCE_RUNNING_RESPONSE=0x0019,
    /*Read detail of a sequence in specify zone*/
    FUNC_READ_DETAIL_OF_A_SEQUENCE_IN_SPECIFY_ZONE=0X0014,
    FUNC_READ_DETAIL_OF_A_SEQUENCE_IN_SPECIFY_ZONE_RESPONSE=0X0015,
    /*Modify detail of a sequence in specify zone*/
    FUNC_MODIFY_DETAIL_OF_A_SEQUENCE_IN_SPECIFY_ZONE=0X0016,
    FUNC_MODIFY_DETAIL_OF_A_SEQUENCE_IN_SPECIFY_ZONE_RESPONSE=0X0017,
    /*Read scene No. of all zones running*/
    FUNC_READ_SCENE_NOof_ALL_ZONES_RUNNING=0xF078,
    FUNC_READ_SCENE_NOof_ALL_ZONES_RUNNING_RESPONSE=0xF079,
    /*Read sequence No. of specify zone running*/
    FUNC_READ_SEQUENCE_NOof_SPECIFY_ZONE_RUNNING=0XE014,
    FUNC_READ_SEQUENCE_NOof_SPECIFY_ZONE_RUNNING_RESPONSE=0xE015,

    /*Forwardly report status*/
    FUNC_FORWARDLY_REPORT_STATUS=0xEFFF,
    /*Read device remark*/
    FUNC_READ_DEVICE_REMARK=0x000E,
    FUNC_READ_DEVICE_REMARK_RESPONSE=0x000F,
    /*Read mac address*/
    FUNC_READ_MAC_ADDRESS=0xF003,
    FUNC_READ_MAC_ADDRESS_RESPONSE=0xF004,
    /*Modify ID by mac address*/
    FUNC_MODIFY_ID_BY_MAC_ADDRESS=0xF005,
    FUNC_MODIFY_ID_BY_MAC_ADDRESS_RESPONSE=0xF006,
    /*Modify address button*/
    FUNC_MODIFY_ADDRESS_BUTTON=0xE5F7,
    FUNC_MODIFY_ADDRESS_BUTTON_RESPONSE=0xE5F8,
    /*Detect address button*/
    FUNC_DETECT_ADDRESS_BUTTON=0xE5F5,
    FUNC_DETECT_ADDRESS_BUTTON_RESPONSE=0xE5F6,
    /*Read specify device on line*/
    FUNC_READ_SPECIFY_DEVICE_ON_LINE=0xF065,
    FUNC_READ_SPECIFY_DEVICE_ON_LINE_RESPONSE=0xF066,
    /*Write device remark*/
    FUNC_WRITE_DEVICE_REMARK=0x0010,
    FUNC_WRITE_DEVICE_REMARK_RESPONSE=0x0011,
    /*Read filmwave version*/
    FUNC_READ_FILMWAVE_VERSION=0xEFFD,
    FUNC_READ_FILMWAVE_VERSION_RESPONSE=0xEFFE,
    /*Read scene configuration*/
    FUNC_READ_SCENE_CONFIGURATION=0x0000,
    FUNC_READ_SCENE_CONFIGURATION_RESPONSE=0x0001,
    /*Modify scene configuration*/
    FUNC_MODIFY_SCENE_CONFIGURATION=0x0008,
    FUNC_MODIFY_SCENE_CONFIGURATION_RESPONSE=0x0009,
    /*Read limit of every channel*/
    FUNC_READ_LIMIT_OF_EVERY_CHANNEL=0xF016,
    FUNC_READ_LIMIT_OF_EVERY_CHANNEL_RESPONSE=0xF017,
    /*Read max level of every channal*/
    FUNC_READ_MAX_LEVEL_OF_EVERY_CHANNEL=0xF020,
    FUNC_READ_MAX_LEVEL_OF_EVERY_CHANNEL_RESPONSE=0xF021

} function;

////////////////////////////////////////////////////////////////////////////////
// CRC Table
////////////////////////////////////////////////////////////////////////////////
uint16_t const CRC_TAB[]={               /* CRC tab */
                                         0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
                                         0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
                                         0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
                                         0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
                                         0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
                                         0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
                                         0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
                                         0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
                                         0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
                                         0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
                                         0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
                                         0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
                                         0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
                                         0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
                                         0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
                                         0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
                                         0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
                                         0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
                                         0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
                                         0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
                                         0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
                                         0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
                                         0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
                                         0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
                                         0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
                                         0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
                                         0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
                                         0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
                                         0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
                                         0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
                                         0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
                                         0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
                         };

class SmartBusRas
{
private:
    uint8_t _u8SerialPort;
    uint16_t BufferRec[40];
    ////////////////////////////////////////////////////////////////////////////////
    // Purpose:    Enable data reception
    // Inputs:     None
    // Outputs:    None
    ////////////////////////////////////////////////////////////////////////////////
    void RCV_ON(void);
    void RCV_OFF(void);

    ////////////////////////////////////////////////////////////////////////////////
    // Purpose:    Puts a character onto the serial line
    // Inputs:     Character
    // Outputs:    None
    ////////////////////////////////////////////////////////////////////////////////
    void Sbus_serial_putc(char c);
    void Sbus_serial_send_start(void);
    void Sbus_serial_send_stop(int mode);
public:

    //set Original ID
    void setOriginalID(char SetOriginalSubnetID, char SetOriginalDeviceID, int SetOriginaldeviceType, char SetQTYOfChannel);

    SmartBusRas();
    void SmartBus_init(int baud);
    ////////////////////////////////////////////////////////////////////////////////
    // CRC Calculator
    // Input: ot including Start code 0xAA,0xAA
    ////////////////////////////////////////////////////////////////////////////////
    void Pack_crc(char *ptr, char len);
    char Check_crc(char *ptr, char len);

    /*SBUS Master Functions*/
    ////////////////////////////////////////////////////////////////////////////////
    // Purpose:    Send a message over the RS485 bus
    // Inputs:     1) TargetSubnetID, TargetDeviceID: The destination address
    //             2) ChannelNo: The number of bytes of channel
    //             3) BrightnessLevel: The Brightness Level 0 is off, 100 is on
    //             4) Runningtime: The Running time 0-3600s
    ////////////////////////////////////////////////////////////////////////////////
    //Detect Address
    void detectAddress(void);


    //Modify Address
    void modifyAddress(char Target_SubnetID, char Target_DeviceID, char NewSubnetID, char NewDeviceID);


    //Single Channel Control
    void singleChannelControl(char Target_SubnetID, char Target_DeviceID, char Channel_No, char Brightness_Level, uint16_t Running_Time);

    //Scene Control
    void sceneControl(char Target_SubnetID, char Target_DeviceID, char Zone_No, char Scene_No);

    //Sequence Control
    void sequenceControl(char Target_SubnetID, char Target_DeviceID, char Area_No, char Sequence_No);

    //Read status of channel
    void readStatusOfChannel (char Target_SubnetID, char Target_DeviceID);

    //Reversing Control
    void reversingControl(char Target_SubnetID, char Target_DeviceID, char ChannelNo, char Reserved,uint16_t RunningTime);

    //Forwardly	Report Status
    void forwardlyReportStatus(char QTYOfZones,char StatusOfZone1, char StatusOfZone2, char QTYOfChannel);

    //Read scene configuration
    void readSceneConfiguration(char Target_SubnetID, char Target_DeviceID, char AreaNo, char SceneNo);

    //Modify scene configuration
    void modifySceneConfiguration(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char SceneNo, uint16_t RunningTimeScene, char *ChannelIntensity);

    //Curtain control enabled
    void curtainControlEnabled(char Target_SubnetID, char Target_DeviceID);

    //Read remark of zone
    void readRemarkOfZone(char Target_SubnetID, char Target_DeviceID, char ZoneNumber);

    //Write remark of zone
    void writeRemarkOfZone(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char *ZoneRemark);

    //Make zone of relay
    void makeZoneOfRelay(char Target_SubnetID, char Target_DeviceID,char ZoneNo, char FlagChannel1, char FlagChannel2, char MaxChannel);

    //Read channel remark
    void readChannelRemark(char Target_SubnetID, char Target_DeviceID, char ChannelNo);

    //Write channel remark
    void writeChannelRemark(char Target_SubnetID, char Target_DeviceID, char ChannelNo, char *ChannelRemark);

    //Read channel load type
    void readChannelLoadType(char Target_SubnetID, char Target_DeviceID);

    //Modify channel load type
    void modifyChannelLoadType(char Target_SubnetID, char Target_DeviceID, char *ChannelLoadType);

    //Read delay of turn on channel
    void readDelayOfTurnOnChannel(char Target_SubnetID, char Target_DeviceID);

    //Modify delay of turn on channel
    void modifyDelayOfTurnOnChannel(char Target_SubnetID, char Target_DeviceID, char *DelayOfTurnOnChannel);

    //Read safeguard time of channel
    void readSafeguardTimeOfChannel(char Target_SubnetID, char Target_DeviceID);

    //Modify safeguard time of channel
    void modifySafeguardTimeOfChannel(char Target_SubnetID, char Target_DeviceID, char *SafeguardTimeOfChannel);

    //Read setting of zones
    void readSettingOfZone(char Target_SubnetID, char Target_DeviceID);

    //Read remark of specify scene of specify zone
    void readRemarkOfSpecifySceneOfSpecifyZone(char Target_SubnetID, char Target_DeviceID);

    //Write remark of specify scene of specify zone
    void writeRemarkOfSpecifySceneOfSpecifyZone(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char SceneNo, char *RemarkOfCurrentScene);

    //Read type of zone when power on
    void readTypeOfZoneWhenPowerOn(char Target_SubnetID, char Target_DeviceID);

    //Write type of zone when power on
    void writeTypeOfZoneWhenPowerOn(char Target_SubnetID, char Target_DeviceID, char *TypeOfZoneWhenPowerOn);

    //Read scene No. of every zone when power on
    void readSceneNoOfEveryZoneWhenPowerOn(char Target_SubnetID, char Target_DeviceID);

    //Modify scene No. of every zone when power on
    void modifySceneNoOfEveryZoneWhenPowerOn(char Target_SubnetID, char Target_DeviceID, char *SceneNoOfEveryZoneWhenPowerOn);

    //Read remark of specify sequence
    void readRemarkOfSpecifySequence(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char SequenceNo);

    //Modify remark of specify sequence
    void modifyRemarkOfSpecifySequence(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char SequenceNo, char *RemarkOfCurrentSequence);

    //Read setting of sequence running
    void readSettingOfSequenceRunning(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char SequenceNo);

    //Modify setting of sequence running
    void modifySettingOfSequenceRunning(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char SequenceNo, char ModelOfRunning, char QYTOfSequence, char QYTOfTimeOfSequence);

    //Read detail of a sequence in specify zone
    void readDetailOfSequenceInSpecifyZone(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char SequenceNo, char StepNo);

    //Modify detail of a sequence in specify zone
    void modifyDetailOfSequenceInSpecifyZone(char Target_SubnetID, char Target_DeviceID, char ZoneNo, char SequenceNo, char StepNo, char SceneNo, uint16_t TimeCurrentStepWillStop);

    //Read scene No. of all zones running
    void readSceneNoOfAllZonesRunning(char Target_SubnetID, char Target_DeviceID);

    //Read sequence No. of specify zone running
    void readSequenceNoOfSpecifyZoneRunning(char Target_SubnetID, char Target_DeviceID, char ZoneNo);

    //Read limit of every channel

    //Read max level of every channel

    //Read device remark
    void readDeviceRemark(char Target_SubnetID, char Target_DeviceID);

    //Read filmwave version

    //Read mac address
    void readMacAddress(char Target_SubnetID, char Target_DeviceID);

    //Modify ID by mac address
    void modifyIDByMacAddress(char Target_SubnetID, char Target_DeviceID, char *MacAddress, char SubnetChange, char DeviceChange);

    //Modify address button
    void modifyAddressButtonResponse(char Target_SubnetID, char Target_DeviceID);

    //Detect address button
    void detectAddressButtonResponse(char Target_SubnetID, char Target_DeviceID);

    //Write device remark
    void writeDeviceRemark(char Target_SubnetID, char Target_DeviceID, char *DeviceRemark);

    //Read specify device in line
    void readSpecifyDeviceOnLineResponse(char Target_SubnetID, char Target_DeviceID);

    void Received(void);

    uint16_t ReceivedData(int index);
    char lenghtData(void);

    void clrearBufferRec(void);

};

#endif // SMARTBUS_H
