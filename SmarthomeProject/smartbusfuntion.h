#ifndef SMARTBUSFUNTION_H
#define SMARTBUSFUNTION_H

#include <QString>

#define OriginalSubnetID    0x01
#define OriginalDeviceID    0xFE
#define OriginaldeviceType  0xFFFE
#define QTYOfChannel        8

typedef enum _functionUART{
    SCENE_CONTROL=1,
    SINGLE_CHANNEL_CONTROL=2,
    MODIFY_SCENE_CONFIGURATION=3,
    READ_STATUS_OF_CHANNEL=4,

    //Devices
    BUTTON_4=0x011A,
    RELAY_8CHANNEL=0x0017,
    DIMMER_8CHANNEL=0x0259
    }functionUART;


class SmartBusFuntion
{
public:
    SmartBusFuntion();
    void ReadValueRandom(void);
    char getButtoValue(int index);
    char getRelayValue(int index);
    void InitSmartBus(int baund);

    char findValue(QString temp ,int point);

    //Reset buffer
    void resetBufferReceived(void);

    //allow transmit
    char getAllowTransmit(void);
    void setAllowTransmit(char value);

    //function transmit
    char getFunctionTrasmit(void);
    void setFunctionTrasmit(char index);

    //funtion finished
    void setFinishReceived(char Value);
    char getFinishReceived(void);

    //function new event
    int getNewEvent(void);
    void setNewEvent(int value);

    //get value event
    int getRelayValueEvent(int index);
    int getDimmervalueEvent(int index);

    //Scene control
    void SceneControlTCP(QString resultFromTCP);
    void SceneControlUART(char SubnetID, char RTUID,char AreaNo,char SceneNo);
    char getSeneControlResponse(int index);
    char getSceneControlValue(int index);

    //Single channel control
    void SingleChannelControlTCP(QString resultFromTCP);
    void SingleChannelControlUART(char SubnetID,char RTUID, char ChannelNo, char Value);
    char getSingleChannelControlValue(int index);
    char getSingleChanneControlResponse(int index);

    //modify scene configuration
    void ModifySceneConfigurationTCP(QString resultFromTCP);
    void ModifySceneConfigurationUART(char SubnetID, char RTUID, char AreaNo, char SceneNo, char *value);
    char getModifySceneConfigurationValue(int index);
    char getModifySceneConfigurationResponse(int index);

    //Read status of channel
    void ReadStatusOfChannelTCP(QString resultFromTCP);
    void ReadStatusOfChannelUART(char SubnetID, char RTUID);
    char getReadStatusOfChannelValue(int index);
    char getReadStatusOfChannelResponse(int index);

};

#endif // SMARTBUSFUNTION_H
